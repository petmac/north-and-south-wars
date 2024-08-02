use std::{error::Error, fs::File, io::Write, path::PathBuf};

use clap::Parser;
use tiled::Loader;

#[derive(Parser)]
struct Cli {
    input_tmx_path: PathBuf,
    output_map_path: PathBuf,
}

const MAX_MAP_WIDTH: usize = 30;
const MAX_MAP_HEIGHT: usize = 20;
const MAX_UNITS: usize = 32;

#[derive(Clone, Copy)]
struct TileCoords {
    column: u8,
    row: u8,
}

struct Map {
    width: u8,
    height: u8,
    tile_indices: [[u8; MAX_MAP_WIDTH]; MAX_MAP_HEIGHT],
    unit_count: u8,
    units: [TileCoords; MAX_UNITS],
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
    let mut units = [TileCoords { column: 0, row: 0 }; MAX_UNITS];
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
                    let unit_index = layer_tile.id() as u8;
                    let coords = TileCoords {
                        column: tile_col as u8,
                        row: tile_row as u8,
                    };
                    units[unit_count] = coords;
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
        output_file.write(&[unit.column, unit.row])?;
    }

    Ok(())
}
