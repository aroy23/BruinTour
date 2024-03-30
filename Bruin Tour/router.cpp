#include "router.h"
#include "geotools.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
using namespace std;

Router::Router(const GeoDatabaseBase& geo_db)
: m_geodb(geo_db)
{}

Router::~Router()
{}

vector<GeoPoint> Router::route(const GeoPoint &pt1, const GeoPoint &pt2) const
{
    vector<GeoPoint> path;
    if(gpEqual(pt1, pt2))
    {
        path.push_back(pt1);
        return path;
    }
    unordered_map<string, GeoPoint> locationOfPreviousWayPoint;
    unordered_map<string, double> gScore;
    unordered_map<string, double> fScore;
    priority_queue<pair<double, string> , vector<pair<double, string>>, greater<pair<double, string>>> fscorePriority;
    
    fscorePriority.push(make_pair(fScore[pt1.to_string()], pt1.to_string()));
    fScore[pt1.to_string()] = distance_earth_miles(pt1, pt2);
    gScore[pt1.to_string()] = 0;
    
    while(!fscorePriority.empty())
    {
        string coordinates = fscorePriority.top().second;
        size_t comma = coordinates.find(',');
        string latitude = coordinates.substr(0, comma);
        string longitude = coordinates.substr(comma + 1);
        GeoPoint current = GeoPoint(latitude, longitude);
        if(gpEqual(current, pt2))
        {
            break;
        }
        fscorePriority.pop();
        vector<GeoPoint> neighbor = m_geodb.get_connected_points(current);
        vector<GeoPoint>::iterator p;
        for(p = neighbor.begin(); p != neighbor.end(); p++)
        {
            double newPathToNeighbhor = gScore[current.to_string()] + distance_earth_miles(current, (*p));
            if(gScore.find((*p).to_string()) == gScore.end() || newPathToNeighbhor < gScore[(*p).to_string()])
            {
                locationOfPreviousWayPoint[(*p).to_string()] = current;
                gScore[(*p).to_string()] = newPathToNeighbhor;
                fScore[(*p).to_string()] = newPathToNeighbhor + distance_earth_miles((*p), pt2);
                fscorePriority.push(make_pair(fScore[(*p).to_string()], (*p).to_string()));
            }
        }
    }
    if(locationOfPreviousWayPoint.find(pt2.to_string()) == locationOfPreviousWayPoint.end())
    {
        return path;
    }
    GeoPoint last = pt2;
    while(!gpEqual(last, pt1))
    {
        path.push_back(last);
        last = locationOfPreviousWayPoint[last.to_string()];
    }
    path.push_back(pt1);
    reverse(path.begin(), path.end());
    
    return path;
}

bool Router::gpEqual(const GeoPoint &p1, const GeoPoint &p2) const
{
    return (p1.sLatitude == p2.sLatitude) && (p1.sLongitude == p2.sLongitude);
}
