use std::path::PathBuf;

use clap::Parser;

#[derive(Parser)]
struct Cli {
    input_tmx_path: PathBuf,
    output_map_path: PathBuf,
}

fn main() {
    let cli = Cli::parse();

    println!("Input:  {}", cli.input_tmx_path.display());
    println!("Output: {}", cli.output_map_path.display());
}
