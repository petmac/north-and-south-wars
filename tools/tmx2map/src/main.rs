use std::{error::Error, fs::File, io::Write, path::PathBuf};

use clap::Parser;
use tiled::Loader;

#[derive(Parser)]
struct Cli {
    input_tmx_path: PathBuf,
    output_map_path: PathBuf,
}

const MAX_MAP_WIDTH: usize = 20;
const MAX_MAP_HEIGHT: usize = 16;
const MAX_UNITS: usize = 32;

#[derive(Clone, Copy)]
struct TileCoords {
    column: u8,
    row: u8,
}

#[derive(Clone, Copy)]
#[repr(u8)]
enum Force {
    North,
    South,
}

impl TryFrom<u8> for Force {
    type Error = &'static str;

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        match value {
            0 => Ok(Force::North),
            1 => Ok(Force::South),
            _ => Err("Unhandled force value"),
        }
    }
}

#[derive(Clone, Copy)]
#[repr(u8)]
enum UnitType {
    Infantry,
    Mech,
    LightTank,
    MediumTank,
    Count,
}
impl TryFrom<u8> for UnitType {
    type Error = &'static str;

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        match value {
            0 => Ok(UnitType::Infantry),
            1 => Ok(UnitType::Mech),
            2 => Ok(UnitType::LightTank),
            3 => Ok(UnitType::MediumTank),
            _ => Err("Unhandled unit type value"),
        }
    }
}

#[derive(Clone, Copy)]
struct MapUnit {
    coords: TileCoords,
    force: Force,
    unit_type: UnitType,
}

struct Map {
    width: u8,
    height: u8,
    tile_indices: [[u8; MAX_MAP_WIDTH]; MAX_MAP_HEIGHT],
    unit_count: u8,
    units: [MapUnit; MAX_UNITS],
}

fn main() -> Result<(), Box<dyn Error>> {
    let cli = Cli::parse();

    println!("Input:  {}", cli.input_tmx_path.display());
    println!("Output: {}", cli.output_map_path.display());

    let mut loader = Loader::new();
    let tmx = loader.load_tmx_map(cli.input_tmx_path)?;
    let mut tile_layers = tmx.layers().filter_map(|layer| layer.as_tile_layer());
    let tiles_layer = tile_layers.next().ok_or("No tiles layer in map")?;
    let units_layer = tile_layers.next().ok_or("No units layer in map")?;

    let width = tiles_layer.width().ok_or("Layer has no width")? as usize;
    let height = tiles_layer.height().ok_or("Layer has no height")? as usize;
    let mut tile_indices: [[u8; MAX_MAP_WIDTH]; MAX_MAP_HEIGHT] =
        [[0; MAX_MAP_WIDTH]; MAX_MAP_HEIGHT];
    let mut units = [MapUnit {
        coords: TileCoords { column: 0, row: 0 },
        force: Force::North,
        unit_type: UnitType::Infantry,
    }; MAX_UNITS];
    let mut unit_count = 0;

    for tile_row in 0..height {
        for tile_col in 0..width {
            let tiles_layer_tile = match tiles_layer.get_tile(tile_col as i32, tile_row as i32) {
                Some(layer_tile) => layer_tile,
                None => continue,
            };
            let tile_index = tiles_layer_tile.id() as u8;
            tile_indices[tile_row][tile_col] = tile_index;

            match units_layer.get_tile(tile_col as i32, tile_row as i32) {
                Some(layer_tile) => {
                    let unit_tile_index = layer_tile.id() as usize;
                    let coords = TileCoords {
                        column: tile_col as u8,
                        row: tile_row as u8,
                    };
                    let force_index = unit_tile_index / UnitType::Count as usize;
                    let force = (force_index as u8).try_into()?;
                    let unit_type_index = unit_tile_index % UnitType::Count as usize;
                    let unit_type = (unit_type_index as u8).try_into()?;
                    let map_unit = MapUnit {
                        coords,
                        force,
                        unit_type,
                    };
                    units[unit_count] = map_unit;
                    unit_count += 1;
                }
                None => {}
            };
        }
    }

    let map = Map {
        width: width as u8,
        height: height as u8,
        tile_indices,
        unit_count: unit_count as u8,
        units,
    };

    let mut output_file = File::create(cli.output_map_path)?;
    output_file.write(&[map.width, map.height])?;
    for row in &map.tile_indices {
        output_file.write(row)?;
    }
    output_file.write(&[map.unit_count])?;
    for unit in &map.units {
        output_file.write(&[
            unit.coords.column,
            unit.coords.row,
            unit.force as u8,
            unit.unit_type as u8,
        ])?;
    }

    Ok(())
}
