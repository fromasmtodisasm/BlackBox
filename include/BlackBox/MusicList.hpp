#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <vector>
#include <SFML/Audio.hpp>
#include <memory>

class MusicList
{
  std::vector<std::unique_ptr<sf::Music>> m_Playlist;
  int m_CurrentTrack;
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

void MusicList::setRootPath(const std::string &RootPath)
{
  m_RootPath = RootPath;
}

float MusicList::getVolume() const
{
  return m_Volume;
}

MusicList::MusicList()
{
  m_CurrentTrack = 0;
  m_Volume = 10.f;
}

bool MusicList::addTrack(std::string path)
{
  auto track = std::make_unique<sf::Music>();
  if (track->openFromFile(m_RootPath + path))
  {
    m_Playlist.push_back(std::move(track));
    return true;
  }
  return false;
}

bool MusicList::next()
{
  m_Playlist[m_CurrentTrack]->pause();
  if (m_CurrentTrack == m_Playlist.size() - 1)
    m_CurrentTrack = 0;
  else {
    m_CurrentTrack++;
  }
  m_Playlist[m_CurrentTrack]->setVolume(m_Volume);
  m_Playlist[m_CurrentTrack]->play();
  return  true;
}

bool MusicList::prev()
{
  m_Playlist[m_CurrentTrack]->pause();
  ;
  if (--m_CurrentTrack < 0 )
    m_CurrentTrack = m_Playlist.size() - 1;
  m_Playlist[m_CurrentTrack]->setVolume(m_Volume);
  m_Playlist[m_CurrentTrack]->play();
  return  true;
}

void MusicList::play()
{
  if (m_Playlist.size() != 0)
  {
    m_Playlist[m_CurrentTrack]->setVolume(m_Volume);
    m_Playlist[m_CurrentTrack]->play();
  }
}

void MusicList::pause()
{
  m_Playlist[m_CurrentTrack]->pause();
}

void MusicList::stop()
{
  m_Playlist[m_CurrentTrack]->stop();
}

void MusicList::setVolume(float volume)
{
  m_Volume = volume;
  m_Playlist[m_CurrentTrack]->setVolume(volume);
}
