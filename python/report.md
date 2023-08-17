# Report on Current NN Progress

## Preprocess Dataset

Use min-max scaling for easier data processing

Choose `n_bins = 8` to form a `8 * 8` 2D histogram.

## Model

Use a three-layer MLPs.
```
NeuralNetwork(
  (MLPs): Sequential(
    (0): Linear(in_features=68, out_features=128, bias=True)
    (1): ReLU()
    (2): Linear(in_features=128, out_features=128, bias=True)
    (3): ReLU()
    (4): Linear(in_features=128, out_features=5, bias=True)
    (5): ReLU()
    (6): Linear(in_features=5, out_features=1, bias=True)
  )
)
```
## Result

Very bad. The model stops learning at around epoch 100, and the R2 score is negative and the loss is very high. Need to discuss.