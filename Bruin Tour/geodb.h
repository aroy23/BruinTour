#ifndef GEODB_H
#define GEODB_H

#include "base_classes.h"
#include "hashmap.h"

class GeoDatabase: public GeoDatabaseBase
{
public:
    GeoDatabase();
    virtual ~GeoDatabase();
    virtual bool load(const std::string& map_data_file);
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    struct StreetSegment
    {
        std::string m_name;
        GeoPoint m_start;
        GeoPoint m_end;
    };
    
    struct PointOfInterest
    {
        std::string m_name;
        GeoPoint m_point;
    };
    
    HashMap<PointOfInterest> m_poiMap;
    HashMap<std::vector<GeoPoint>> m_pointMap;
    HashMap<std::string> m_streetNameMap;
    
    bool m_gettingName;
    bool m_gettingCoords;
    bool m_gettingPOI;
    bool m_processPOI;
    bool m_firstPopBack;
};

#endif // GEODB_H
