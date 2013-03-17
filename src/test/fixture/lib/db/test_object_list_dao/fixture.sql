CREATE TABLE `objects`(`id` INTEGER PRIMARY KEY, `name` TEXT NOT NULL);
CREATE UNIQUE INDEX `index_name` ON `objects`(`name`);
CREATE TABLE `meshes`(`id` TEXT PRIMARY KEY, `object_id` INTEGER NOT NULL);

INSERT INTO `objects`(`id`, `name`) VALUES(1, 'wood');
INSERT INTO `meshes`(`id`, `object_id`) VALUES('ffe5e445838806b8789ef72d237c1fe0bc12fa78', 1);
INSERT INTO `meshes`(`id`, `object_id`) VALUES('fe94d5a321e01919451c16fe7998946d83c1935b', 1);
