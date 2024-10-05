# Kalman Filter

시간 $t$에 로봇의 위치를 $x_t$, 로봇의 센서로부터 관측한 값을 $z_t$, 로봇의 제어입력을 $u_t$라고 하면 이를 통해 모션 모델(motion model)과 관측 모델(observation model)을 정의할 수 있다. 이때, 모션 모델과 관측 모델은 선형이어야 한다는 제약 조건이 있다. 모션 모델과 관측 모델은 아래와 같다.

# Model Equations
- Motion Model
$$ x_t=F_tx_{t-1}+B_tu_t+w_t $$
- Observation Model
$$ z_t=H_tx_t+v_t $$

$ x_t $: 모델의 상태 변수(state variable)

$ u_t $: 모델의 입력(input data)

$ z_t $: 모델의 관측값(measurement)

$ F_t $: 모델의 상태천이(state transition) 행렬

$ B_t $: 모델의 입력 $u_t$가 주어졌을 때 $u_t$를 상태 변수로 변환해주는 행렬

$ H_t $: 모델의 관측(observation) 행렬

$ w_t \sim \textit{N}(0,\textbf{Q}_t)$: 모션 모델의 노이즈, $\textbf{Q}_t$는 $w_t$의 공분산 행렬을 의미한다.

$ v_t \sim \textit{N}(0,\textbf{R}_t)$: 관측 모델의 노이즈, $\textbf{R}_t$는 $v_t$의 공분산 행렬을 의미한다.
  
<br />

# Gaussian belief case
확률변수가 모두 가우시안 분포를 따른다고 가정하면 $p(x_t|x_{t-1},u_t)$, $p(z_t|x_t)$는 다음과 같이 나타낼 수 있다.

$$ p(x_t|x_{t-1},u_t)\sim N(\textbf{F}_tx_{t-1}+B_tu_t, \textbf{Q}_t)  $$
$$ =\frac{1}{\sqrt{det(2\pi \textbf{Q}_t)}}exp(-\frac{1}{2}(x_t- \textbf{F}_tx_{t-1}-B_tu_t)^T \ \textbf{Q}_t^{-1} (x_t- \textbf{F}_tx_{t-1}-B_tu_t)) $$

<br />

$$ p(z_t|x_t) \sim N(\textbf{H}_t x_t\ \textbf{R}_t) $$
$$ =\frac{1}{\sqrt{det(2\pi \textbf{R}_t)}}exp(-\frac{1}{2}(z_t-\textbf{H}_tx_t)^T \textbf{R}_t^{-1} (z_t-\textbf{H}_tx_t)) $$

<br />
<br />

다음으로 칼만 필터를 통해 구해야하는 $\overline{bel}(x_t),bel(x_t)$은 아래와 같이 나타낼 수 있다.

$$Prediction: \overline{bel}(x_t)=\int p(x_t|x_{t-1},u_t)bel(x_{t-1})dx_{t-1}\sim N(\hat{x}_{t|t-1},\textbf{P}_{t|t-1})$$

$$Correction: bel(x_t)=\eta \cdot p(z_t|x_t)\overline{bel}(x_t)\sim N(\hat{x}_{t|t},\textbf{P}_{t|t}) $$

<br />

# Prediction Step Equations 
Prediction은 $\overline{bel}(x_t)$를 구하는 과정이다. $\overline{bel}(x_t)$는 가우시안 분포를 따르므로 평균(State)과 분산(covariance)을 각각 아래와 같이 구할 수 있다.

### Prediction Step Equations 
- Predicted State
$$ \hat{x}_{t|t-1}=F_{t}\hat{x}_{t-1|t-1}+B_t\vec{u}_t $$

- Predicted Covariance
$$ P_{t|t-1}=F_{t}P_{t-1|t-1}F_{t}^T+Q $$

<br />

# Correction Step 
Correction은 $bel(x_t)$를 구하는 과정을 말한다. $bel(x_t)$ 또한 가우시안 분포를 따르므로 평균과 분산을 각각 아래와 같이 구할 수 있다. 

### Correction Step Equations 
- Kalman Gain
$$ K_t=P_{t|t-1}H_t^T(H_tP_{t|t-1}H_t^T+R_t)^{-1} $$

- Correction State
$$ \hat{x}_{t|t}=\hat{x}_{t|t-1}+K_t(z_t-H_t\hat{x}_{t|t-1}) $$

- Correction Covariance
$$ P_{t|t}=(I-K_t H_t)P_{t|t-1} $$