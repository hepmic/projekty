#include <cstdlib>
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <string>

using namespace std;

string AllowedChars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

class selection
{
public:
  static int fitness(string candidate)
  {
    assert(target.length() == candidate.length());
    int FitnessValue = 0;
    for (int i = 0; i < target.length(); ++i)
    {
      int target_pos = AllowedChars.find(target[i]);
      int candidate_pos = AllowedChars.find(candidate[i]);
      int difference = abs(target_pos - candidate_pos);
      FitnessValue -= min(difference, int(AllowedChars.length()) - difference);
    }
    return FitnessValue;
  }
  static int target_length() { return target.length(); }
private:
  static string target;        //cel jest ukryty
};
string selection::target = "Chromosome representationChromosome representation";

bool less_fit(string const& s1, string const& s2)
{
  return selection::fitness(s1) < selection::fitness(s2);
}
void move_char(char& c, int distance)
{
  while (distance < 0)
    distance += AllowedChars.length();
  int char_pos = AllowedChars.find(c);
  c = AllowedChars[(char_pos + distance) % AllowedChars.length()];
}
string mutate(string parent, double MutationRate)
{
  for (int i = 0; i < parent.length(); ++i)
    if (rand()/(RAND_MAX + 1.0) < MutationRate)
    {
      int distance = rand() % 3 + 1;
      if(rand()%2 == 0)
        move_char(parent[i], distance);
      else
        move_char(parent[i], -distance);
    }
  return parent;
}

int main()
{
  int const C = 100;
  srand(time(0));
  string parent;
  for (int i = 0; i < selection::target_length(); ++i)
  {
    parent += AllowedChars[rand() % AllowedChars.length()];      //losowanie poczatkowego stringa
  }
  int const InitialFitness = selection::fitness(parent);

  for(int fitness = InitialFitness, j = 1;
      fitness < 0;
      fitness = selection::fitness(parent), j++)
  {
    cout << "Generacja nr:" << j << "\t" << parent << ": " << fitness << "\n";
    double const MutationRate = 0.03 + (0.9*fitness)/InitialFitness;
    vector<string> childs;
    childs.reserve(C+1);
    childs.push_back(parent);
    for (int i = 0; i < C; ++i)
      childs.push_back(mutate(parent, MutationRate));
    parent = *max_element(childs.begin(), childs.end(), less_fit);
  }
  cout << "final string: " << parent << "\n";
}
