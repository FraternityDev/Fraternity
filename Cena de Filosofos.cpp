#include <iostream>
#include <mutex>
#include <random>
#include <thread>

const unsigned int numFilo = 5;//numero de filosofos
const unsigned int numAcc = 10; //numero de acciones

/*
int tiempo(int bajo, int alto) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribution(bajo, alto);
  return distribution(gen);
}
*/

class DiningPhilosophers;

struct Philosopher {
  Philosopher(unsigned int id, std::string name, unsigned int number_actions,
              DiningPhilosophers* table)
      : id_(id), name_(name), number_actions_(number_actions), table_(table) {}

  void operator()() {
    for (unsigned int i = 0; i < number_actions_; ++i) {
      comer();
      // if (tiempo(1, 2) < 2) {
      //   pensar();
      // } else {
      //   comer();
      // }
    }
  }

  void pensar();
  void comer();

  unsigned int id_;
  std::string name_;
  unsigned int number_actions_;
  DiningPhilosophers* table_;
};

class DiningPhilosophers {
 public:
  DiningPhilosophers() {
    std::thread* philosophers[numFilo];
    std::string philosopher_names[] = {
        "Socrates", "Platon", "Aristoteles", "Descartes", "Tomas",
    };
    for (unsigned int i = 0; i < numFilo; ++i) {
      philosophers[i] = new std::thread(
          Philosopher(i, philosopher_names[i], numAcc, this));
    }
    for (unsigned int i = 0; i < numFilo; ++i) {
      philosophers[i]->join();
    }
  }

  void PickFork(unsigned int id) {
    forks_[id].lock();
    forks_[(id + 1) % numFilo].lock();
  }

  void LeaveFork(unsigned int id) {
    forks_[id].unlock();
    forks_[(id + 1) % numFilo].unlock();
  }

 private:
  std::mutex forks_[numFilo];
};

void Philosopher::pensar() {
  std::cout << name_ + " esta pensando." << std::endl;
  //std::this_thread::sleep_for(std::chrono::seconds(tiempo(1, 6)));
}

void Philosopher::comer() {
  table_->PickFork(id_);
  std::cout << name_ + " esta comiendo." << std::endl;
  //std::this_thread::sleep_for(std::chrono::seconds(tiempo(1, 6)));
  table_->LeaveFork(id_);
}

int main() {
  DiningPhilosophers table;
  return 0;
}
