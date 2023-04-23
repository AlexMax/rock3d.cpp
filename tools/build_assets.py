#
# rock3d.cpp: A 3D game engine for making retro FPS games
# Copyright (C) 2018 Lexi Mayfield <alexmax2742@gmail.com>
#

from dataclasses import dataclass
from enum import StrEnum
from pathlib import Path
from typing import NoReturn
import os
import subprocess
import sys

ROOT_DIR = Path(__file__).parent.parent
ASSETS_DIR = ROOT_DIR / "build" / "assets"
VCPKG_INSTALLED_DIR = ROOT_DIR / "build" / "vcpkg_installed"

SHADERC_EXE = VCPKG_INSTALLED_DIR / "x64-windows" / "tools" / "bgfx" / "shaderc.exe"
SHADERC_INCLUDE_DIR = VCPKG_INSTALLED_DIR / "x64-windows" / "include" / "bgfx"

PathParam = os.PathLike | bytes | str


class ShaderType(StrEnum):
    vertex = "vertex"
    fragment = "fragment"
    compute = "compute"


@dataclass
class Shader:
    path: Path
    type: ShaderType


def error(msg: str) -> NoReturn:
    print(f"build_assets.py: {msg}", file=sys.stderr)
    sys.exit(1)


def build_shader(
    output_file: PathParam,
    input_file: PathParam,
    shader_type: ShaderType,
    shader_profile: str,
) -> None:
    args: list[str] = [
        str(SHADERC_EXE),
        "-i",
        str(SHADERC_INCLUDE_DIR),
        "-f",
        str(input_file),
        "-o",
        str(output_file),
        "--type",
        str(shader_type),
        "--profile",
        shader_profile,
    ]

    proc = subprocess.run(args)
    if proc.returncode > 0:
        error("Shader failed to compile")


def build_shaders() -> None:
    out_base_path = ASSETS_DIR / "shaders"
    out_profile = "spirv15-12"
    (out_base_path / out_profile).mkdir(parents=True, exist_ok=True)

    shaders: list[Shader] = [
        Shader(
            ROOT_DIR / "rock3d" / "r3d" / "shaders" / "world" / "vert.sc",
            ShaderType.vertex,
        ),
        Shader(
            ROOT_DIR / "rock3d" / "r3d" / "shaders" / "world" / "frag.sc",
            ShaderType.fragment,
        ),
        Shader(
            ROOT_DIR / "rocked" / "shaders" / "imgui" / "vert.sc", ShaderType.vertex
        ),
        Shader(
            ROOT_DIR / "rocked" / "shaders" / "imgui" / "frag.sc", ShaderType.fragment
        ),
    ]

    for shader in shaders:
        if not shader.path.exists():
            error("File does not exist")

        out_filename = str(shader.path.relative_to(ROOT_DIR))
        out_filename = out_filename.replace("/", "_")
        out_filename = out_filename.replace("\\", "_")
        out_filename += ".bin"
        out_path = out_base_path / out_profile / out_filename

        build_shader(out_path, shader.path, shader.type, out_profile)


def main() -> None:
    ASSETS_DIR.mkdir(parents=True, exist_ok=True)

    build_shaders()


if __name__ == "__main__":
    main()
