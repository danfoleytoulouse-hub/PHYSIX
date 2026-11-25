# PHYSIX — NumWorks app

Native C app for NumWorks using the EADK.

## Compile in one click

1. Open the **Actions** tab of this repository.
2. Select **Build PHYSIX NumWorks app**.
3. Click **Run workflow** (branch: main), then **Run workflow**.
4. When it finishes green, open the run and download the **PHYSIX-NumWorks-nwa** artifact.
5. Extract the ZIP to get `PHYSIX.nwa`, then install it with the NumWorks app uploader.

The workflow installs the ARM toolchain, restores the app icon from its text-encoded source, runs `make clean build`, and uploads the compiled `.nwa`.

## Current app content

The current native version is a compact study prototype with sections for electrochemical cells, combustion/CO2, units/methods, solved examples, and quiz answers. It does not yet include every detail of the earlier Python study guide.

## Build locally

Requires `arm-none-eabi-gcc`, Node.js/npm, and make. Run `make build`; output is `output/PHYSIX.nwa`.
