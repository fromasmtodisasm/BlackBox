#include <Game.hpp>
#include <TagPoint.hpp>

//////////////////////////////////////////////////////////////////////
// Inserts a tag-point in the list.
ITagPoint* CGame::CreateTagPoint(const string& name, const Vec3& pos, const Vec3& angles)
{
  // create new one
  CTagPoint* pNewPoint = new CTagPoint(this);
  pNewPoint->OverrideName(name.c_str());
  pNewPoint->SetPos(pos);
  pNewPoint->SetAngles(angles);

  // insert it into the map
  m_mapTagPoints.insert(TagPointMap::iterator::value_type(name, pNewPoint));
  return (ITagPoint*)pNewPoint;
}

//////////////////////////////////////////////////////////////////////
// Remove tag-point.
void CGame::RemoveTagPoint(ITagPoint* pPoint)
{
  TagPointMap::iterator ti;
  // find and delete tag-point
  for (ti = m_mapTagPoints.begin(); ti != m_mapTagPoints.end(); ti++)
  {
    if (ti->second == pPoint)
    {
      m_mapTagPoints.erase(ti);
      pPoint->Release();
      return;
    }
  }
}

//////////////////////////////////////////////////////////////////////
// Retrieve a tag-point by its name
ITagPoint* CGame::GetTagPoint(const string& name)
{
  TagPointMap::iterator ti;
  // find and return tag-point
  if ((ti = m_mapTagPoints.find(name)) == m_mapTagPoints.end())
    return 0;
  else
    return (ITagPoint*)ti->second;
}

//////////////////////////////////////////////////////////////////////
bool CGame::RenameTagPoint(const string& oldname, const string& newname)
{
  TagPointMap::iterator ti;
  // find tag-point
  if ((ti = m_mapTagPoints.find(oldname)) != m_mapTagPoints.end())
  {
    // does the new name already exist ?
    if (m_mapTagPoints.find(newname) == m_mapTagPoints.end())
    {
      // change name
      CTagPoint* pPoint = ti->second;
      pPoint->OverrideName(newname);

      m_mapTagPoints.erase(oldname);
      m_mapTagPoints.insert(TagPointMap::iterator::value_type(newname, pPoint));

      return true;
    }
    else
      return false;
  }

  return false;
}
