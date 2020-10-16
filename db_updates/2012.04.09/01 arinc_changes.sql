
  DROP TABLE ARINC_CHANGES;

  CREATE TABLE "ARINC_CHANGES" 
   (	"ID" NUMBER NOT NULL ENABLE, 
	"STATEMENT" VARCHAR2(4000 CHAR) NOT NULL ENABLE, 
	"TIME" TIMESTAMP (6) DEFAULT systimestamp NOT NULL ENABLE, 
	 CONSTRAINT "ARINC_CHANGES_PK" PRIMARY KEY ("ID")
  USING INDEX PCTFREE 10 INITRANS 2 MAXTRANS 255 
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS"  ENABLE
   ) PCTFREE 10 PCTUSED 40 INITRANS 1 MAXTRANS 255 NOCOMPRESS LOGGING
  STORAGE(INITIAL 65536 NEXT 1048576 MINEXTENTS 1 MAXEXTENTS 2147483645
  PCTINCREASE 0 FREELISTS 1 FREELIST GROUPS 1 BUFFER_POOL DEFAULT)
  TABLESPACE "USERS" ;
 
   CREATE SEQUENCE  "ARINC_CHANGES_SEQ"  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 CACHE 20 NOORDER  NOCYCLE ;
 
  CREATE OR REPLACE TRIGGER "ARINC_CHANGES_TRG" BEFORE INSERT ON ARINC_CHANGES 
FOR EACH ROW 
BEGIN
  <<COLUMN_SEQUENCES>>
  BEGIN
    SELECT ARINC_CHANGES_SEQ.NEXTVAL INTO :NEW.ID FROM DUAL;
  END COLUMN_SEQUENCES;
END;
/
ALTER TRIGGER "ARINC_CHANGES_TRG" ENABLE;




CREATE OR REPLACE PACKAGE zone_pkg as 

  TYPE zone_rec_type IS REF CURSOR RETURN zone%ROWTYPE;
  TYPE airport_rec_type IS REF CURSOR RETURN airport%ROWTYPE;
  TYPE point_rec_type IS REF CURSOR RETURN point%ROWTYPE;
  TYPE route_rec_type IS REF CURSOR RETURN route%ROWTYPE;
  TYPE runway_rec_type IS REF CURSOR RETURN runway%ROWTYPE;
  TYPE ssa_rec_type IS REF CURSOR RETURN ssa%ROWTYPE;
  TYPE restriction_rec_type IS REF CURSOR RETURN restriction%ROWTYPE;
  TYPE restriction_user_rec_type IS REF CURSOR RETURN restriction_user%ROWTYPE;
  
  PROCEDURE get_cur_zone(idval IN zone.id%TYPE, cursor_ret OUT zone_pkg.zone_rec_type);
  PROCEDURE get_cur_airport(idval IN airport.id%TYPE, cursor_ret OUT zone_pkg.airport_rec_type);
  PROCEDURE get_cur_point(idval IN point.id%TYPE, cursor_ret OUT zone_pkg.point_rec_type);
  PROCEDURE get_cur_route(idval IN route.id%TYPE, cursor_ret OUT zone_pkg.route_rec_type);
  PROCEDURE get_cur_runway(idval IN runway.id%TYPE, cursor_ret OUT zone_pkg.runway_rec_type);
  PROCEDURE get_cur_ssa(idval IN ssa.id%TYPE, cursor_ret OUT zone_pkg.ssa_rec_type);
  PROCEDURE get_cur_restriction(idval IN restriction.id%TYPE, cursor_ret OUT zone_pkg.restriction_rec_type);
  PROCEDURE get_cur_restriction_user(idval IN restriction_user.id%TYPE, cursor_ret OUT zone_pkg.restriction_user_rec_type);
  
end zone_pkg;
/
 
CREATE OR REPLACE PACKAGE BODY zone_pkg as

  PROCEDURE get_cur_zone(idval IN zone.id%TYPE, cursor_ret OUT zone_pkg.zone_rec_type) as
  pragma autonomous_transaction;
    local_cur zone_rec_type;
  begin
    OPEN local_cur FOR SELECT * FROM zone WHERE id = idval;
    cursor_ret := local_cur;
    rollback;
  end get_cur_zone;

  PROCEDURE get_cur_airport(idval IN airport.id%TYPE, cursor_ret OUT zone_pkg.airport_rec_type) as
  pragma autonomous_transaction;
    local_cur airport_rec_type;
  begin
    OPEN local_cur FOR SELECT * FROM airport WHERE id = idval;
    cursor_ret := local_cur;
    rollback;
  end get_cur_airport;

  PROCEDURE get_cur_point(idval IN point.id%TYPE, cursor_ret OUT zone_pkg.point_rec_type) as
  pragma autonomous_transaction;
    local_cur point_rec_type;
  begin
    OPEN local_cur FOR SELECT * FROM point WHERE id = idval;
    cursor_ret := local_cur;
    rollback;
  end get_cur_point;

  PROCEDURE get_cur_route(idval IN route.id%TYPE, cursor_ret OUT zone_pkg.route_rec_type) as
  pragma autonomous_transaction;
    local_cur route_rec_type;
  begin
    OPEN local_cur FOR SELECT * FROM route WHERE id = idval;
    cursor_ret := local_cur;
    rollback;
  end get_cur_route;

  PROCEDURE get_cur_runway(idval IN runway.id%TYPE, cursor_ret OUT zone_pkg.runway_rec_type) as
  pragma autonomous_transaction;
    local_cur runway_rec_type;
  begin
    OPEN local_cur FOR SELECT * FROM runway WHERE id = idval;
    cursor_ret := local_cur;
    rollback;
  end get_cur_runway;

  PROCEDURE get_cur_ssa(idval IN ssa.id%TYPE, cursor_ret OUT zone_pkg.ssa_rec_type) as
  pragma autonomous_transaction;
    local_cur ssa_rec_type;
  begin
    OPEN local_cur FOR SELECT * FROM ssa WHERE id = idval;
    cursor_ret := local_cur;
    rollback;
  end get_cur_ssa;

  PROCEDURE get_cur_restriction(idval IN restriction.id%TYPE, cursor_ret OUT zone_pkg.restriction_rec_type) as
  pragma autonomous_transaction;
    local_cur restriction_rec_type;
  begin
    OPEN local_cur FOR SELECT * FROM restriction WHERE id = idval;
    cursor_ret := local_cur;
    rollback;
  end get_cur_restriction;

  PROCEDURE get_cur_restriction_user(idval IN restriction_user.id%TYPE, cursor_ret OUT zone_pkg.restriction_user_rec_type) as
  pragma autonomous_transaction;
    local_cur restriction_user_rec_type;
  begin
    OPEN local_cur FOR SELECT * FROM restriction_user WHERE id = idval;
    cursor_ret := local_cur;
    rollback;
  end get_cur_restriction_user;

end zone_pkg;
/
