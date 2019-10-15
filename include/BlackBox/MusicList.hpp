#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <vector>
#include <SFML/Audio.hpp>
#include <memory>

class MusicList
{
  std::vector<std::unique_ptr<sf::Music>> m_Playlist;
  size_t m_CurrentTrack;
  float m_Volume;
  std::string m_RootPath;

public:
  MusicList();
  bool addTrack(std::string	path);
  bool next();
  bool prev();
  void play();
  void pause();
  void stop();
  void setVolume(float volume);
  void setRootPath(const std::string &RootPath);
  float getVolume() const;
};

#endif // MUSICLIST_H


