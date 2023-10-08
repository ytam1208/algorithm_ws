#include <iostream>
#include <vector>

using namespace std;

// 이산 베이즈 필터 클래스 정의
class DiscreteBayesFilter {
public:
    DiscreteBayesFilter(int num_states) : num_states_(num_states) {
        // 초기화: 모든 상태의 확률을 동일하게 설정
        belief_.resize(num_states, 1.0 / num_states);
    }

    // 업데이트 함수: 라이다 정보를 이용하여 추정값 업데이트
    void Update(vector<double> likelihood) {
        // 베이즈 업데이트
        for (int i = 0; i < num_states_; ++i) {
            belief_[i] *= likelihood[i];
        }

        // 정규화
        double sum = 0.0;
        for (int i = 0; i < num_states_; ++i) {
            sum += belief_[i];
        }
        for (int i = 0; i < num_states_; ++i) {
            belief_[i] /= sum;
        }
    }

    // 현재 추정값 출력
    void PrintBelief() {
        for (int i = 0; i < num_states_; ++i) {
            cout << "State " << i << ": " << belief_[i] << endl;
        }
    }

private:
    int num_states_; // 상태 개수
    vector<double> belief_; // 상태 확률 분포
};

int main() {
    int num_states = 5; // 예시로 5개의 상태를 가정
    DiscreteBayesFilter filter(num_states);

    // 라이다 정보로부터 계산된 우도 (likelihood)
    vector<double> likelihood = {0.1, 0.2, 0.4, 0.2, 0.1};

    // 업데이트 실행
    filter.Update(likelihood);

    // 현재 추정값 출력
    filter.PrintBelief();

    return 0;
}