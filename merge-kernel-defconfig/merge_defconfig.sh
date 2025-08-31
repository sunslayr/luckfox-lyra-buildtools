#!/bin/bash

 ./../lyra-sdk/kernel/scripts/kconfig/merge_config.sh -m -O ./out rk3506_luckfox_defconfig rk3506-merge.config && mv ./out/.config ./out/merged_defconfig
