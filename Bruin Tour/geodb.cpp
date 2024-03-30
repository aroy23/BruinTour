#include "geodb.h"
#include "geotools.h"
#include "hashmap.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

GeoDatabase::GeoDatabase()
{
    m_gettingName = true;
    m_gettingCoords = false;
    m_gettingPOI = false;
    m_processPOI = false;
    m_firstPopBack = true;
}

GeoDatabase::~GeoDatabase()
{}

bool GeoDatabase::load(const std::string &map_data_file)
{
    ifstream infile(map_data_file);
    if(!infile)
    {
        return false;
    }
    string line;
    StreetSegment segment;
    PointOfInterest p;
    GeoPoint g1;
    GeoPoint g2;
    int poi = 0;
    while (getline(infile, line))
    {
        istringstream iss(line);
        if(m_gettingName)
        {
            segment.m_name = line;
            m_gettingName = false;
            m_gettingCoords = true;
            continue;
        }
        else if(m_gettingCoords)
        {
            iss >> segment.m_start.sLatitude >> segment.m_start.sLongitude >> segment.m_end.sLatitude >> segment.m_end.sLongitude;
            g1.sLatitude = segment.m_start.sLatitude; g1.sLongitude = segment.m_start.sLongitude;
            g2.sLatitude = segment.m_end.sLatitude; g2.sLongitude = segment.m_end.sLongitude;
            m_streetNameMap[g1.to_string()+g2.to_string()] = segment.m_name;
            m_streetNameMap[g2.to_string()+g1.to_string()] = segment.m_name;
            GeoPoint g3 = GeoPoint(g1.sLatitude, g1.sLongitude);
            GeoPoint g4 = GeoPoint(g2.sLatitude, g2.sLongitude);
            m_pointMap[g1.to_string()].push_back(g4);
            m_pointMap[g2.to_string()].push_back(g3);
            m_firstPopBack = true;
            m_gettingCoords = false;
            m_gettingPOI = true;
            continue;
        }
        else if(m_gettingPOI)
        {
            iss >> poi;
            if(poi > 0)
            {
                m_gettingPOI = false;
                m_processPOI = true;
                continue;
            }
            else
            {
                m_gettingPOI = false;
                m_gettingName = true;
                continue;
            }
        }
        else if(m_processPOI)
        {
            if(poi != 0)
            {
                GeoPoint x = GeoPoint(segment.m_start.sLatitude, segment.m_start.sLongitude);
                GeoPoint y = GeoPoint(segment.m_end.sLatitude, segment.m_end.sLongitude);
                GeoPoint mp = midpoint(x, y);
                getline(iss, p.m_name, '|');
                iss >> p.m_point.sLatitude >> p.m_point.sLongitude;
                if(m_firstPopBack)
                {
                    StreetSegment s1;
                    StreetSegment s2;
                    s1.m_start = segment.m_start;
                    s1.m_end = mp;
                    g1.sLatitude = s1.m_start.sLatitude; g1.sLongitude = s1.m_start.sLongitude;
                    g2.sLatitude = s1.m_end.sLatitude; g2.sLongitude = s1.m_end.sLongitude;
                    m_streetNameMap[g1.to_string()+g2.to_string()] = segment.m_name;
                    m_streetNameMap[g2.to_string()+g1.to_string()] = segment.m_name;
                    GeoPoint g3 = GeoPoint(g1.sLatitude, g1.sLongitude);
                    GeoPoint g4 = GeoPoint(g2.sLatitude, g2.sLongitude);
                    m_pointMap[g1.to_string()].push_back(g4);
                    m_pointMap[g2.to_string()].push_back(g3);
                    s2.m_start = segment.m_end;
                    s2.m_end = mp;
                    g1.sLatitude = s2.m_start.sLatitude; g1.sLongitude = s2.m_start.sLongitude;
                    g2.sLatitude = s2.m_end.sLatitude; g2.sLongitude = s2.m_end.sLongitude;
                    m_streetNameMap[g1.to_string()+g2.to_string()] = segment.m_name;
                    m_streetNameMap[g2.to_string()+g1.to_string()] = segment.m_name;
                    GeoPoint g5 = GeoPoint(g1.sLatitude, g1.sLongitude);
                    GeoPoint g6 = GeoPoint(g2.sLatitude, g2.sLongitude);
                    m_pointMap[g1.to_string()].push_back(g6);
                    m_pointMap[g2.to_string()].push_back(g5);
                    m_firstPopBack = false;
                }
                StreetSegment s3;
                s3.m_start = p.m_point;
                s3.m_end = mp;
                g1.sLatitude = s3.m_start.sLatitude; g1.sLongitude = s3.m_start.sLongitude;
                g2.sLatitude = s3.m_end.sLatitude; g2.sLongitude = s3.m_end.sLongitude;
                GeoPoint g3 = GeoPoint(g1.sLatitude, g1.sLongitude);
                GeoPoint g4 = GeoPoint(g2.sLatitude, g2.sLongitude);
                if(m_streetNameMap.find(g1.to_string()+g2.to_string()) == nullptr && m_streetNameMap.find(g2.to_string()+g1.to_string()) == nullptr)
                {
                    m_pointMap[g1.to_string()].push_back(g4);
                    m_pointMap[g2.to_string()].push_back(g3);
                }
                m_streetNameMap[g1.to_string()+g2.to_string()] = "a path";
                m_streetNameMap[g2.to_string()+g1.to_string()] = "a path";
                GeoPoint p1 = GeoPoint(p.m_point.sLatitude, p.m_point.sLongitude);
                p.m_point = p1;
                m_poiMap.insert(p.m_name, p);
                poi--;
                if(poi == 0)
                {
                    m_processPOI = false;
                    m_gettingName = true;
                }
                continue;
            }
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
    const PointOfInterest* poiObject = m_poiMap.find(poi);
    if(poiObject != nullptr)
    {
        point = poiObject->m_point;
        return true;
    }
    return false;
}

vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
    const string key = pt.to_string();
    const vector<GeoPoint>* connectedPoints = m_pointMap.find(key);
    if (connectedPoints != nullptr)
    {
        return *connectedPoints;
    }
    else
    {
        vector<GeoPoint> v;
        return v;
    }
}

string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
    const string* s = m_streetNameMap.find(pt1.to_string()+pt2.to_string());
    if(s != nullptr)
    {
        return *s;
    }
    return "";
}
