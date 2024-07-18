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

struct Map {
    width: u8,
    height: u8,
    tile_indices: [[u8; MAX_MAP_WIDTH]; MAX_MAP_HEIGHT],
}

fn main() -> Result<(), Box<dyn Error>> {
    let cli = Cli::parse();

    println!("Input:  {}", cli.input_tmx_path.display());
    println!("Output: {}", cli.output_map_path.display());

    let mut loader = Loader::new();
    let tmx = loader.load_tmx_map(cli.input_tmx_path)?;
    let layer = tmx
        .layers()
        .filter_map(|layer| layer.as_tile_layer())
        .next()
        .ok_or("No tile layers in map")?;

    let width = layer.width().ok_or("Layer has no width")? as usize;
    let height = layer.height().ok_or("Layer has no height")? as usize;
    let mut tile_indices: [[u8; MAX_MAP_WIDTH]; MAX_MAP_HEIGHT] =
        [[0; MAX_MAP_WIDTH]; MAX_MAP_HEIGHT];

    for tile_row in 0..height {
        for tile_col in 0..width {
            let layer_tile = match layer.get_tile(tile_col as i32, tile_row as i32) {
                Some(layer_tile) => layer_tile,
                None => continue,
            };
            let tile_index = layer_tile.id() as u8;
            tile_indices[tile_row][tile_col] = tile_index;
        }
    }

    let map = Map {
        width: width as u8,
        height: height as u8,
        tile_indices,
    };

    let mut output_file = File::create(cli.output_map_path)?;
    output_file.write(&[map.width])?;
    output_file.write(&[map.height])?;
    for row in &map.tile_indices {
        output_file.write(row)?;
    }

    Ok(())
}
