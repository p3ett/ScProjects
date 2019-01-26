DROP DATABASE Auctions;
CREATE DATABASE Auctions;

USE Auctions;

CREATE TABLE IF NOT EXISTS `user` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `approved` TINYINT(1) NOT NULL DEFAULT 0,
  `username` VARCHAR(60) NOT NULL,
  `password` VARCHAR(60) NOT NULL,
  `firstname` VARCHAR(80) NOT NULL,
  `lastname` VARCHAR(80) NOT NULL,
  `email` VARCHAR(100) NOT NULL,
  `phone` VARCHAR(30) NOT NULL,
  `country` VARCHAR(60) NOT NULL,
  `location` VARCHAR(60) NOT NULL,
  `taxnumber` VARCHAR(40) NOT NULL,
  `seller_rating` INT NOT NULL DEFAULT 0,
  `bidder_rating` INT NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`, `seller_rating`),
  UNIQUE INDEX `name_UNIQUE` (`username` ASC))
ENGINE = InnoDB;

CREATE TABLE IF NOT EXISTS `auction` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `user_id` INT NOT NULL,
  `name` VARCHAR(45) NOT NULL,
  `first_bid` DECIMAL(20,2) NOT NULL DEFAULT 0,
  `buy_price` DECIMAL(20,2) NULL,
  `description` TEXT(10000) NOT NULL,
  `location` VARCHAR(200) NULL,
  `country` VARCHAR(100) NULL,
  `started` DATETIME NULL DEFAULT NULL,
  `ended` DATETIME NULL DEFAULT NULL,
  `sold_to` INT NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_auction_user_idx` (`user_id` ASC),
  INDEX `fk_auction_user1_idx` (`sold_to` ASC),
  CONSTRAINT `fk_auction_user`
    FOREIGN KEY (`user_id`)
    REFERENCES `user` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_auction_user1`
    FOREIGN KEY (`sold_to`)
    REFERENCES `user` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE TABLE IF NOT EXISTS `bid` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `user_id` INT NOT NULL,
  `auction_id` INT NOT NULL,
  `time` DATETIME NOT NULL,
  `amount` DECIMAL(20,2) NOT NULL,
  INDEX `fk_user_has_auction_auction1_idx` (`auction_id` ASC),
  INDEX `fk_user_has_auction_user1_idx` (`user_id` ASC),
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_user_has_auction_user1`
    FOREIGN KEY (`user_id`)
    REFERENCES `user` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_user_has_auction_auction1`
    FOREIGN KEY (`auction_id`)
    REFERENCES `auction` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE TABLE IF NOT EXISTS `category` (
  `name` VARCHAR(100) NOT NULL,
  `auction_id` INT NOT NULL,
  PRIMARY KEY (`name`, `auction_id`),
  INDEX `fk_category_auction1_idx` (`auction_id` ASC),
  CONSTRAINT `fk_category_auction1`
    FOREIGN KEY (`auction_id`)
    REFERENCES `auction` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE TABLE IF NOT EXISTS `image` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `auction_id` INT NOT NULL,
  `filename` VARCHAR(100) NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_image_auction1_idx` (`auction_id` ASC),
  CONSTRAINT `fk_image_auction1`
    FOREIGN KEY (`auction_id`)
    REFERENCES `auction` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SELECT * FROM user;
SELECT * FROM auction;
SELECT * FROM bid;

DELETE FROM bid WHERE id < 5
