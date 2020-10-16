DELETE FROM restriction_user;

ALTER TABLE restriction_user ADD COLUMN actual smallint;
ALTER TABLE restriction_user ALTER COLUMN actual SET NOT NULL;
COMMENT ON COLUMN restriction_user.actual IS '3 положения:
1. птичка снята - зона не отображается на карте и не активна (0)
2. птичка серая - зона отображается и активна согласно заданного интервала времени (1)
3. птичка жирная - зона отображается и Всегда активна не зависимо от заданного времени. (2)';



ALTER TABLE restriction_user DISABLE TRIGGER restriction_user_delete_trg;