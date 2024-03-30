#include "base_classes.h"
#include "tour_generator.h"
#include "stops.h"
#include <vector>
#include <algorithm>
#include "geotools.h"
#include <iostream>
using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
: m_geodb(geodb), m_router(router)
{}

TourGenerator::~TourGenerator()
{}

string TourGenerator::directionOfTravel(double angle) const
{
    if(0 <= angle && angle <= 22.5)
    {
        return "east";
    }
    else if(22.5 <= angle && angle < 67.5)
    {
        return "northeast";
    }
    else if(67.5 <= angle && angle < 112.5)
    {
        return "north";
    }
    else if(112.5 <= angle && angle < 157.5)
    {
        return "northwest";
    }
    else if(157.5 <= angle && angle < 202.5)
    {
        return "west";
    }
    else if(202.5 <= angle && angle < 247.5)
    {
        return "southwest";
    }
    else if(247.5 <= angle && angle < 292.5)
    {
        return "south";
    }
    else if(292.5 <= angle && angle < 337.5)
    {
        return "southeast";
    }
    else
    {
        return "east";
    }
}

vector<TourCommand> TourGenerator::generate_tour(const Stops &stops) const
{
    vector<TourCommand> result;
    bool brokeBecauseInvalid = false;
    for(int i = 0; i < stops.size(); i++)
    {
        string poi;
        string talkingPoints;
        stops.get_poi_data(i, poi, talkingPoints);
        TourCommand commentary;
        commentary.init_commentary(poi, talkingPoints);
        result.push_back(commentary);
        GeoPoint current;
        if(!m_geodb.get_poi_location(poi, current))
        {
            brokeBecauseInvalid = true;
            break;
        }
        if(stops.get_poi_data(i+1, poi, talkingPoints))
        {
            GeoPoint next;
            if(!m_geodb.get_poi_location(poi, next))
            {
                brokeBecauseInvalid = true;
                break;
            }
            vector<GeoPoint> route = m_router.route(current, next);
            if(route.size() == 0)
            {
                brokeBecauseInvalid = true;
                break;
            }
            for(int j = 0; j < route.size()-1; j++)
            {
                GeoPoint curr = route[j];
                GeoPoint next = route[j+1];
                TourCommand proceed;
                proceed.init_proceed(directionOfTravel(angle_of_line(curr, next)), m_geodb.get_street_name(curr, next), distance_earth_miles(curr, next), curr, next);
                result.push_back(proceed);
                if(j+2 < route.size())
                {
                    GeoPoint afterNext = route[j+2];
                    if(m_geodb.get_street_name(curr, next) != m_geodb.get_street_name(next, afterNext) && angle_of_turn(curr, next, afterNext) != 0 && directionOfTravel(angle_of_line(curr, next)) != directionOfTravel(angle_of_line(next, afterNext)))
                    {
                        if(angle_of_turn(curr, next, afterNext) >= 1 && angle_of_turn(curr, next, afterNext) < 180)
                        {
                            TourCommand turn;
                            turn.init_turn("left", m_geodb.get_street_name(next, afterNext));
                            result.push_back(turn);
                        }
                        else if(angle_of_turn(curr, next, afterNext) >= 180 && angle_of_turn(curr, next, afterNext) <= 359)
                        {
                            TourCommand turn;
                            turn.init_turn("right", m_geodb.get_street_name(next, afterNext));
                            result.push_back(turn);
                        }
                    }
                }
            }
        }
        else
        {
            return result;
        }
    }
    if(brokeBecauseInvalid)
    {
        vector<TourCommand> empty;
        return empty;
    }
    return result;
}
