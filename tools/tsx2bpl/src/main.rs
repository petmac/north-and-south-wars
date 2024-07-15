use std::{error::Error, fs::File, io::Write, path::PathBuf};

use clap::Parser;
use tiled::{Loader, Tileset};

#[derive(Parser)]
struct Cli {
    input_tsx_path: PathBuf,
    output_bpl_path: PathBuf,
}

struct ImageAtlas {
    images: Vec<Image>,
}

const TILE_WIDTH_WORDS: usize = 1;
const TILE_WIDTH: usize = TILE_WIDTH_WORDS * 16;
const TILE_HEIGHT: usize = TILE_WIDTH;
const TILE_DEPTH: usize = 5;

#[derive(Default)]
struct Image {
    rows: [Row; TILE_HEIGHT],
}

#[derive(Default)]
struct Row {
    plane_rows: [PlaneRow; TILE_DEPTH],
}

#[derive(Default)]
struct PlaneRow {
    words: [Word; TILE_WIDTH_WORDS],
}

#[derive(Default)]
// Amiga is Big Endian
struct Word {
    hi: u8,
    lo: u8,
}

fn main() -> Result<(), Box<dyn Error>> {
    let cli = Cli::parse();

    println!("Input:  {}", cli.input_tsx_path.display());
    println!("Output: {}", cli.output_bpl_path.display());

    let mut loader = Loader::new();
    let tileset = loader.load_tsx_tileset(cli.input_tsx_path)?;
    let atlas = load_tileset(&tileset)?;

    let mut output_file = File::create(cli.output_bpl_path)?;
    for image in &atlas.images {
        for row in &image.rows {
            for plane_row in &row.plane_rows {
                for word in &plane_row.words {
                    output_file.write(&[word.hi])?;
                    output_file.write(&[word.lo])?;
                }
            }
        }
    }

    Ok(())
}

fn load_tileset(tileset: &Tileset) -> Result<ImageAtlas, Box<dyn Error>> {
    let image = tileset.image.as_ref().ok_or("Tileset has no image")?;
    let path = image.source.canonicalize()?;
    let decoder = png::Decoder::new(File::open(path)?);
    let mut reader = decoder.read_info()?;
    let mut buf = vec![0; reader.output_buffer_size()];
    let info = reader.next_frame(&mut buf)?;
    let bytes = &buf[..info.buffer_size()];
    let image_rows: Vec<&[u8]> = bytes.chunks_exact(image.width as usize).collect();
    let offset_between_tiles = (tileset.margin + (tileset.spacing * tileset.tile_width)) as usize;

    let mut images = Vec::new();
    for tile_index in 0..tileset.tilecount as usize {
        let tile_col = tile_index % tileset.columns as usize;
        let tile_row = tile_index / tileset.columns as usize;
        let tile_x1 = tile_col * offset_between_tiles;
        let tile_y1 = tile_row * offset_between_tiles;
        let tile_x2 = tile_x1 + tileset.tile_width as usize;
        let tile_y2 = tile_y1 + tileset.tile_height as usize;
        let subimage_rows = &image_rows[tile_y1..tile_y2];
        let subimage: Vec<&[u8]> = subimage_rows
            .iter()
            .map(|&row| &row[tile_x1..tile_x2])
            .collect();
        let atlas_image = convert_image(&subimage);
        images.push(atlas_image);
    }

    Ok(ImageAtlas { images })
}

fn convert_image(input_image: &[&[u8]]) -> Image {
    let mut output = Image::default();

    for y in 0..TILE_HEIGHT {
        let output_row = &mut output.rows[y];

        for x in 0..TILE_WIDTH {
            let word_index = x / 16;
            let color_index = input_image[y][x] as usize;
            let hi_byte = (x & 15) < 8;
            let shift_in_byte = 7 - (x & 7);

            for plane_index in 0..TILE_DEPTH {
                let color_mask = (1 as usize) << plane_index;
                if (color_index & color_mask) == 0 {
                    continue;
                }

                let output_plane_row = &mut output_row.plane_rows[plane_index];
                let output_word = &mut output_plane_row.words[word_index];
                let output_byte = &mut if hi_byte {
                    output_word.hi
                } else {
                    output_word.lo
                };
                *output_byte |= (1 as u8) << shift_in_byte;
            }
        }
    }

    output
}
