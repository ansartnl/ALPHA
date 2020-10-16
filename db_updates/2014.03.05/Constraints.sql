ALTER TABLE ROUTE_POINT
ADD CONSTRAINT ROUTE_POINT_UK1 UNIQUE 
(
  POINT_ID 
, ROUTE_ID 
)
ENABLE;

ALTER TABLE ZONE_AIRPORT
ADD CONSTRAINT ZONE_AIRPORT_UK1 UNIQUE 
(
  ZONE_ID 
, AIRPORT_ID 
)
ENABLE;

ALTER TABLE ZONE_POINT
ADD CONSTRAINT ZONE_POINT_UK1 UNIQUE 
(
  ZONE_ID 
, POINT_ID 
)
ENABLE;