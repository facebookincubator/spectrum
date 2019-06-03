#!/bin/bash
# Copyright (c) Facebook, Inc. and its affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
#
# Extracted from https://github.com/facebook/react-native/blob/master/scripts/android-setup.sh

echo "Waiting for Android Virtual Device to finish booting..."
bootanim=""
export PATH=$(dirname $(dirname $(command -v android)))/platform-tools:$PATH
until [[ "$bootanim" =~ "stopped" ]]; do
  sleep 5
  bootanim=$(adb -e shell getprop init.svc.bootanim 2>&1)
  echo "boot animation status=$bootanim"
done
echo "Android Virtual Device is ready."
