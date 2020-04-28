# Sketch_apr21a

This is a simple code that demonstrate Spresense DNNRT.

## Requirements
1. Spresense main board
1. Spresense tool-chains
1. Spresnese-Arduino IDE

For installation of Arduino-Spresense, please refer to [this official ducument](https://developer.sony.com/develop/spresense/docs/arduino_set_up_en.html).

## Build and run
From Arduino IDE, click on the button __Upload__ .

## Data
[This directory data](./data) contains C-style NNB model and inference dataset.
* In order to understand how to generate the models, please refer to [this Jupyter Notebook](https://github.com/mu-triv/Nnabla-examples/blob/master/nbla_plays.ipynb#Convert-from-binary-NNB-to-C-style-NNB)
* In order to understand how to generate the dataset, please refer to [this Jupyter Notebook](https://github.com/mu-triv/Nnabla-examples/blob/master/nbla_plays.ipynb#Convert-from-binary-raw-images-to-C-style-images)

### Binary to C-style converter
Please refer to this https://github.com/Jamesits/bin2array