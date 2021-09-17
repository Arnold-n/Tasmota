Import("env")
import os
import shutil
import gzip
import pathlib

import tasmotapiolib


def map_gzip(source, target, env):
    # create string with location and file names based on variant
    map_file = tasmotapiolib.get_final_map_path(env)

    if map_file.is_file():
        gzip_file = map_file.with_suffix(".map.gz")

        # check if new target map files exist and remove if necessary
        if gzip_file.is_file():
            gzip_file.unlink()

        # write gzip map file
        with map_file.open("rb") as fp:
            with gzip.open(gzip_file, "wb", compresslevel=9) as f:
                shutil.copyfileobj(fp, f)

        # remove map file
        if map_file.is_file():
            map_file.unlink()


if not tasmotapiolib.is_env_set(tasmotapiolib.DISABLE_MAP_GZ, env):
    env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", [map_gzip])

# gzip only for ESP8266
if env["PIOPLATFORM"] != "espressif32":

    def bin_gzip(source, target, env):
        # create string with location and file names based on variant
        bin_file = tasmotapiolib.get_final_bin_path(env)
        gzip_file = bin_file.with_suffix(".bin.gz")

        # check if new target files exist and remove if necessary
        if os.path.isfile(gzip_file):
            os.remove(gzip_file)

        # write gzip firmware file
        with open(bin_file, "rb") as fp:
            with gzip.open(gzip_file, "wb", compresslevel=9) as f:
                shutil.copyfileobj(fp, f)

        ORG_FIRMWARE_SIZE = bin_file.stat().st_size
        GZ_FIRMWARE_SIZE = gzip_file.stat().st_size

        if ORG_FIRMWARE_SIZE > 995326:
            print(
                "\u001b[31;1m!!! Tasmota firmware size is too big with {} bytes. Max size is 995326 bytes !!! \u001b[0m".format(
                    ORG_FIRMWARE_SIZE
                )
            )
        else:
            print(
                "Compression reduced firmware size by {:.0f}% (was {} bytes, now {} bytes)".format(
                    (GZ_FIRMWARE_SIZE / ORG_FIRMWARE_SIZE) * 100,
                    ORG_FIRMWARE_SIZE,
                    GZ_FIRMWARE_SIZE,
                )
            )

    if not tasmotapiolib.is_env_set(tasmotapiolib.DISABLE_BIN_GZ, env):
        env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", [bin_gzip])
