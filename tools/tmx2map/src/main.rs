use std::{error::Error, path::PathBuf};

use clap::Parser;
use tiled::Loader;

#[derive(Parser)]
struct Cli {
    input_tmx_path: PathBuf,
    output_map_path: PathBuf,
}

fn main() -> Result<(), Box<dyn Error>> {
    let cli = Cli::parse();

    println!("Input:  {}", cli.input_tmx_path.display());
    println!("Output: {}", cli.output_map_path.display());

    let mut loader = Loader::new();
    let _tmx = loader.load_tmx_map(cli.input_tmx_path)?;

    Ok(())
}
