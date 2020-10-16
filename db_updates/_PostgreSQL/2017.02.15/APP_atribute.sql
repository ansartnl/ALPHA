ALTER TABLE sectoring_type ADD COLUMN app boolean;
COMMENT ON COLUMN sectoring_type.app IS 'App attribute';