#ifndef ROUTER_H
#define ROUTER_H

#include "base_classes.h"
#include <unordered_map>
#include "geotools.h"

class Router: public RouterBase
{
public:
    Router(const GeoDatabaseBase& geo_db);
    virtual ~Router();
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:    
    const GeoDatabaseBase& m_geodb;
    bool gpEqual(const GeoPoint& p1, const GeoPoint& p2) const;
};

#endif // ROUTER_H
