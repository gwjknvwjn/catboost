Build according to official CatBoost documentation.

To Run Stochastic Gradient Langevin Dynamics specify --langevin True --diffusion-temperature <value> --model-shrink-rate <value> (corresponds to \beta and \gamma).
For Smooth Loss Approximation (SLA) specify --loss-function UserPerObjMetric:alpha=0.1 (alpha corresponds to \varsigma)
  
Included notebook with synthetic experiments

