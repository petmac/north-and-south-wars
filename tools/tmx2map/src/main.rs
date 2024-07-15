use std::{error::Error, fs::File, path::PathBuf};

use clap::Parser;
use tiled::{Loader, Tileset};

#[derive(Parser)]
struct Cli {
    input_tmx_path: PathBuf,
    output_map_path: PathBuf,
}

struct TilesetImages {
    tiles: Vec<TileImage>,
}

struct TileImage {}

fn main() -> Result<(), Box<dyn Error>> {
    let cli = Cli::parse();

    println!("Input:  {}", cli.input_tmx_path.display());
    println!("Output: {}", cli.output_map_path.display());

    let mut loader = Loader::new();
    let tmx = loader.load_tmx_map(cli.input_tmx_path)?;

    let tileset_images: Vec<TilesetImages> = tmx
        .tilesets()
        .iter()
        .map(|tileset| load_tileset(tileset.as_ref()).unwrap())
        .collect();

    for layer in tmx.layers().filter_map(|layer| layer.as_tile_layer()) {
        let layer_w = layer.width().ok_or("Layer has no width")?;
        let layer_h = layer.height().ok_or("Layer has no height")?;

        for tile_row in 0..layer_h {
            for tile_col in 0..layer_w {
                let layer_tile = match layer.get_tile(tile_col as i32, tile_row as i32) {
                    Some(layer_tile) => layer_tile,
                    None => continue,
                };
                let tileset_images = &tileset_images[layer_tile.tileset_index()];
                let tile_image = &tileset_images.tiles[layer_tile.id() as usize];
                todo!()
            }
        }
    }

    Ok(())
}

fn load_tileset(tileset: &Tileset) -> Result<TilesetImages, Box<dyn Error>> {
    let image = tileset.image.as_ref().ok_or("Tileset has no image")?;
    let path = image.source.canonicalize()?;
    let decoder = png::Decoder::new(File::open(path)?);
    let mut reader = decoder.read_info()?;
    let mut buf = vec![0; reader.output_buffer_size()];
    let info = reader.next_frame(&mut buf)?;
    let bytes = &buf[..info.buffer_size()];
    let image_rows: Vec<&[u8]> = bytes.chunks_exact(image.width as usize).collect();
    let offset_between_tiles = (tileset.margin + (tileset.spacing * tileset.tile_width)) as usize;

    for tile_index in 0..tileset.tilecount as usize {
        let tile_col = tile_index % tileset.columns as usize;
        let tile_row = tile_index / tileset.columns as usize;
        let tile_x1 = tile_col * offset_between_tiles;
        let tile_y1 = tile_row * offset_between_tiles;
        let tile_x2 = tile_x1 + tileset.tile_width as usize;
        let tile_y2 = tile_y1 + tileset.tile_height as usize;
        let subimage_rows = &image_rows[tile_y1..tile_y2];
        let subimage = subimage_rows.iter().map(|&row| &row[tile_x1..tile_x2]);
    }

    Ok(TilesetImages { tiles: Vec::new() })
}
