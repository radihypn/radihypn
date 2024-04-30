#include <Player.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
  Player p;
  p.play("https://listen.moe/opus");
  std::string a;
  std::getline(std::cin, a);
}
