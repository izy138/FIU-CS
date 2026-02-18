-- Drop tables in correct order
DROP TABLE IF EXISTS Reviews;
DROP TABLE IF EXISTS RecipeIngredients;
DROP TABLE IF EXISTS Recipes;
DROP TABLE IF EXISTS Users;
DROP TABLE IF EXISTS Ingredients;
DROP TABLE IF EXISTS Categories;


-- TABLE 1: Categories 
-- look up table for recipe categoreis
CREATE TABLE Categories (
    category_id INT PRIMARY KEY AUTO_INCREMENT,
    category_name VARCHAR(50) NOT NULL UNIQUE,
    category_description TEXT
);

-- TABLE 2: Users
-- stores user account information
CREATE TABLE Users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(100) NOT NULL UNIQUE,
    first_name VARCHAR(50),
    last_name VARCHAR(50),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- TABLE 3: Recipes 
-- tracks recipe info and links to categories and users
CREATE TABLE Recipes (
    recipe_id INT PRIMARY KEY AUTO_INCREMENT,
    recipe_name VARCHAR(100) NOT NULL,
    category_id INT NOT NULL,
    user_id INT NOT NULL,
    recipe_description TEXT,
    dietary_info SET('Gluten-Free', 'Vegan', 'Paleo', 'Keto', 'Vegetarian', 'Dairy-Free', 'Low-Carb', 'Low-Sodium', 'Low-Sugar', 'Low-Fat', 'Low-Calorie'),
    prep_time_minutes INT NOT NULL CHECK (prep_time_minutes > 0),
    cook_time_minutes INT NOT NULL CHECK (cook_time_minutes > 0),
    servings INT NOT NULL DEFAULT 4 CHECK (servings >= 1),
    difficulty_level ENUM('Easy', 'Medium', 'Hard') NOT NULL,
    instructions TEXT NOT NULL,
    times_cooked INT DEFAULT 0,
    is_favorite BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (category_id) REFERENCES Categories(category_id)
        ON DELETE RESTRICT
        ON UPDATE CASCADE
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE
    UNIQUE (user_id, recipe_name)
);

-- TABLE 4: Ingredients
-- tracks individual ingredients and their nutrition info
CREATE TABLE Ingredients (
    ingredient_id INT PRIMARY KEY AUTO_INCREMENT,
    ingredient_name VARCHAR(100) NOT NULL UNIQUE,
    unit_of_measure VARCHAR(20) NOT NULL,
    calories_per_unit DECIMAL(5,2) CHECK (calories_per_unit >= 0),
    protein_per_unit DECIMAL(5,2) CHECK (protein_per_unit >= 0),
    carbs_per_unit DECIMAL(5,2) CHECK (carbs_per_unit >= 0),
    fat_per_unit DECIMAL(5,2) CHECK (fat_per_unit >= 0),
    fiber_per_unit DECIMAL(5,2) CHECK (fiber_per_unit >= 0),
    sugar_per_unit DECIMAL(5,2) CHECK (sugar_per_unit >= 0),
    sodium_per_unit DECIMAL(5,2) CHECK (sodium_per_unit >= 0),
    cholesterol_per_unit DECIMAL(5,2) CHECK (cholesterol_per_unit >= 0),
    saturated_fat_per_unit DECIMAL(5,2) CHECK (saturated_fat_per_unit >= 0)
);

-- TABLE 5: RecipeIngredients 
--stores recipe ingredients and quantities 
-- acts as a junction table linking recipes to their ingredients
CREATE TABLE RecipeIngredients (
    recipe_ingredient_id INT PRIMARY KEY AUTO_INCREMENT,
    recipe_id INT NOT NULL,
    ingredient_id INT NOT NULL,
    quantity DECIMAL(5,2) NOT NULL CHECK (quantity > 0),
    is_optional BOOLEAN DEFAULT FALSE,
    notes VARCHAR(200),
    FOREIGN KEY (recipe_id) REFERENCES Recipes(recipe_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (ingredient_id) REFERENCES Ingredients(ingredient_id)
        ON DELETE RESTRICT
        ON UPDATE CASCADE,
    UNIQUE (recipe_id, ingredient_id)
);

-- TABLE 6: Reviews
--stores user reviews and ratings for recipes
--acts as a junction table linking users to recipes by ratings
CREATE TABLE Reviews (
    review_id INT PRIMARY KEY AUTO_INCREMENT,
    recipe_id INT NOT NULL,
    user_id INT NOT NULL,
    rating DECIMAL(2,1) NOT NULL CHECK (rating >= 1.0 AND rating <= 5.0),
    review_text TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (recipe_id) REFERENCES Recipes(recipe_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    UNIQUE (recipe_id, user_id)
);

-- INTEGRITY TESTS (REQUIRED)

-- 1. Duplicate recipe name for the same user - violates UNIQUE constraint in Recipes table UNIQUE (user_id, recipe_name)
-- Dessert category_id is 4, user_id is 1
-- First insert succeeds:
-- INSERT INTO Recipes (recipe_name, category_id, user_id, prep_time_minutes, cook_time_minutes, servings, difficulty_level, instructions)
-- VALUES ('Chocolate Chip Cookies', 4, 1, 15, 10, 24, 'Easy', 'Mom's recipe');
--
-- Second insert fails - same user has two recipes with the same name:
-- INSERT INTO Recipes (recipe_name, category_id, user_id, prep_time_minutes, cook_time_minutes, servings, difficulty_level, instructions)
-- VALUES ('Chocolate Chip Cookies', 4, 1, 20, 15, 12, 'Easy', 'Grandma's recipe');
-- Expected Error: Duplicate entry for composite key (user_id, recipe_name)

-- 2. Negative nutritional value - violates CHECK constraint in Ingredients table on calories_per_unit DECIMAL(5,2) CHECK (calories_per_unit >= 0),
-- INSERT INTO Ingredients (ingredient_name, unit_of_measure, calories_per_unit, protein_per_unit)
-- VALUES ('Negative Calories Food', 'cup', -100.0, 5.0);
-- Expected Error: Check constraint violated on 'calories_per_unit >= 0'