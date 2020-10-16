update AIRPORT set DECLINATION = 'E0008'
where NAME = 'UTTT'
/
update RUNWAY set latitude = 41.25566542, longitude = 69.25842743
where 
NAME = 'RW08L'
and airport_id in (select id from AIRPORT where NAME = 'UTTT')
/
update RUNWAY set latitude = 41.25388614, longitude = 69.25924185
where 
NAME = 'RW08R'
and airport_id in (select id from AIRPORT where NAME = 'UTTT')
/
update RUNWAY set latitude = 41.25876639, longitude = 69.30357544
where 
NAME = 'RW26L'
and airport_id in (select id from AIRPORT where NAME = 'UTTT')
/
update RUNWAY set latitude = 41.26064485, longitude = 69.30395717
where 
NAME = 'RW26R'
and airport_id in (select id from AIRPORT where NAME = 'UTTT')
/
commit