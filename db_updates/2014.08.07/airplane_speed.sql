ALTER TABLE AIRPLANE_SPEED 
ADD (IAS_CLIMB FLOAT(126) DEFAULT NULL);

ALTER TABLE AIRPLANE_SPEED 
ADD (IAS_CLIMB_MIN FLOAT(126) DEFAULT NULL);

ALTER TABLE AIRPLANE_SPEED 
ADD (IAS_CLIMB_MAX FLOAT(126) DEFAULT NULL);

ALTER TABLE AIRPLANE_SPEED 
ADD (IAS_DESCEND FLOAT(126) DEFAULT NULL);

ALTER TABLE AIRPLANE_SPEED 
ADD (IAS_DESCEND_MIN FLOAT(126) DEFAULT NULL);

ALTER TABLE AIRPLANE_SPEED 
ADD (IAS_DESCEND_MAX FLOAT(126) DEFAULT NULL);
