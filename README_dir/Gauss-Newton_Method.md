# 가우스-뉴턴법의 이해

최소자승법은 주어진 데이터에 가장 잘 맞는 모델의 Parameter를 구하는 방법 중 하나로 이름에서 알 수 있듯이 데이터와의 rasidual의 제곱합을 최소화하도록 모델의 파라미터를 구하는 방법을 의미한다.

</br>

![image](https://user-images.githubusercontent.com/56625848/210483832-81f417c6-957e-45eb-878e-5c6d284c8a18.png)

위 그림에서와 같이 데이터의 분포를 보면 직관적으로 가장 쉽게 데이터들을 잘 표현하는 모델은 직선의 방정식이 생각난다. 

그렇다면 

$$model:y=ax+b$$

$$model\ parameter: a, b$$

</br>

f(x)가 직선 ax+b인 경우에는 아래와 같이 최소자승법 식을 구성할 수 있다.

$$LSM: \ \sum_{i=1}^{n} residual_i^2=\sum_{i=1}^{n} (y_i-f(x_i))^2$$
$$=\sum_{i=1}^{n}(y_i-ax_i-b)^2$$

</br>

하지만 주어진 모델 문제가 선형식이 아닌 경우에는 위와 같은 방법으로 풀기는 어렵다. 즉, 선형 least square 문제가 아닌 비선형 least square 문제로 풀어야 한다. 비선형 문제는 가우스-뉴턴 방법을 이용하면 풀 수 있다. 

</br>

가우스-뉴턴 방법은 1개의 방정식 문제를 다루는 뉴턴 방법을 연립방정식으로 확장한 것으로 생각할 수 있다.

$$f_1(x_1, x_2, x_3)=0$$
$$f_2(x_1, x_2, x_3)=0$$
$$\vdots$$
$$f_n(x_1, x_2, x_3)=0$$

Jacobian Matrix는 다음와 같이 표현할 수 있다.

$$J=\begin{bmatrix}
\frac{\partial f_{1}}{\partial x_{1}}   \frac{\partial f_{1}}{\partial x_{2}}     \frac{\partial f_{1}}{\partial x_{3}}\\\\
\vdots \\\\
\frac{\partial f_{i}}{\partial x_{1}}   \frac{\partial f_{i}}{\partial x_{2}}     \frac{\partial f_{i}}{\partial x_{3}}
\end{bmatrix}$$ 

이를 이용하여 뉴턴 방법을 적용하면 Paramete(X Matrix)의 update는 다음과 같이 구할 수 있다.

$$X_{new}=X-J^{-1}F$$

이때, 일반적으로 Jacobian은 square matrix가 아니므로 Pseudo inverse를 이용하여 다음과 같이 update 수식을 정의할 수 있다.

$$X_{new}=X-{(J^T J)}^{-1}J^TF$$

</br>

# 예제 문제: 파란색 샘플링 데이터를 근사하는 원 방정식 문제를 구하는 문제

</br>

예를들어 아래와 같은 빨간색 원안에 sampling된 반원 데이터(blue points)가 있다. 그렇다면 모델과 모델 파라미터는 아래와 같이 정의된다.


</br>

![image](https://user-images.githubusercontent.com/56625848/210484830-04f47d53-25c0-4158-8fe4-c23cb7faca89.png)

</br>

---

$$model: \sqrt{(x_i-a)^2+(y_i-b)^2}=r$$

</br>

$$model \ parameter: a, b, r$$

$$Goal: a^{optimize}, b^{optimize}, r^{optimize} =argmin_{a,b,r}\sum_{i}^{N}||r-\sqrt{(x_i-a)^2+(y_i-b)^2}||^2$$

</br>


$$LSM: \ residual_i^2=\sqrt{(x_i-a)^2+(y_i-b)^2}-r$$

</br>

i개의 샘플 데이터가 있을때 구해야하는 모델의 파라미터 X는 다음과 같다.

$$X=
 \begin{pmatrix}
  a\\
  b\\
  r
 \end{pmatrix}$$

</br>

따라서, i개의 샘플 데이터를 표현하는 모델 연립방정식은 F는 아래와 같이 표현 할 수 있다. 

 $$F=
 \begin{pmatrix}
  \sqrt{(x_1-a)^2+(y_1-b)^2}-r\\
  \vdots\\
  \sqrt{(x_i-a)^2+(y_i-b)^2}-r\\
 \end{pmatrix}$$
 
</br>

Matrix F의 x에 대한 1차 미분은 다음과 같이 jacobian으로 표현할 수 있다.

 $$Jacobian=
 \begin{pmatrix}
  \frac{a-x_1}{\sqrt{(x_1-a)^2+(y_1-b)^2}} & \frac{b-y_1}{\sqrt{(x_1-a)^2+(y_1-b)^2}} & -1 \\
  \vdots & \vdots & \vdots \\
  \frac{a-x_i}{\sqrt{(x_i-a)^2+(y_i-b)^2}} & \frac{b-y_i}{\sqrt{(x_1-a)^2+(y_1-b)^2}} & -1 \\
 \end{pmatrix}$$
 
</br>

$$ \begin{bmatrix}
a_{new} \\\\
b_{new} \\\\
r_{new}
\end{bmatrix} =\begin{bmatrix}
a \\\\
b \\\\
r
\end{bmatrix} -(J^TJ)^{-1}J^TF$$

최종적으로 fitting이 종료되는 시점은 X Matrix가 더 이상 수렴하지 않거나, 일정 조건까지 수렴했을 경우 종료한다.
