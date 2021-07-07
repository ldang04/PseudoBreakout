/*
Rebounding Ball Project with Brick Wall Modification. 
Author: Linh Dang
*/

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using std::cout; 
using std::endl;
using std::string;
using std::to_string;

// Function prototype declarations. ======================================================================= 

void initializeWall(sf::RectangleShape (&brick)[51]);

void initializePaddle(const int paddleLength, int paddleInitialX, int paddleInitialY, sf::RectangleShape &paddle);

void updatePaddle(int &paddleCurrentX, int &paddleCurrentY, int &paddleRightCurrentX, int paddleInitialX, int paddleInitialY, const int paddleLength, sf::RectangleShape &paddle);

void updateBall(int xChangeRate, int yChangeRate, int &xPosition, int &yPosition); 

int ballCollidedWall(sf::Vector2f ballTopCollision, sf::Vector2f ballBottomCollision, sf::Vector2f ballRightCollision, sf::Vector2f ballLeftCollision, sf::RectangleShape brick[51], int &yChangeRate, int &xChangeRate); 

bool ballCollidedEdge(int xPosition, int yPosition, int diameter);

bool ballCollidedPaddle(sf::Vector2f ballTopCollision, sf::Vector2f ballBottomCollision, sf::Vector2f ballRightCollision, sf::Vector2f ballLeftCollision, sf::RectangleShape paddle); 

void removeBrick(sf::RectangleShape (&brick)[51], int (&collidedBricks)[51], int collidedBrickNum);  

void setRemainingText(); 

void setWin(bool &isPlaying, bool &hasWon); 

void setLoss(bool &isPlaying, bool &hasLost); 

// Main Function ============================================================================================

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "COSC-051-10 Pseudobreakout Project by L. Dang");
    
    // Playing states. 
    bool isPlaying{true}; 
    bool hasWon{false}; 
    bool hasLost{false};
    
    // Ball constants.
    const int radius{10};
    const int diameter{radius*2};
    
    // Initial position and rate variables.
    int xChangeRate{5};
    int yChangeRate{5};
    
    // Initial position of ball's top left point. 
    int xPosition{240}, 
        yPosition{250}; 

    //CircleShape variable (the ball) and corresponding attributes.
    sf::CircleShape ball(radius);
    ball.setFillColor(sf::Color::Green);
    ball.setPosition(xPosition, yPosition);
    
    // RectangleShape variable for the paddle. 
    const int paddleLength{120};
    int paddleInitialX{240};
    int paddleInitialY{555};
    sf::RectangleShape paddle;
    
    initializePaddle(paddleLength, paddleInitialX, paddleInitialY, paddle);
    
    // Paddle current X and Y positions. 
    int paddleCurrentX{paddleInitialX};
    int paddleCurrentY{paddleInitialY};
    int paddleRightCurrentX{paddleCurrentX + paddleLength};
        
    // Set up brick wall. 
    sf::RectangleShape brick[51];
    int collidedBricks[51]; 
    int bricksRemaining{50};
    
    initializeWall(brick);
    
    // Initialize font and text. 
    sf::Font font; 
    
    if (!font.loadFromFile("arial.ttf")) {
    	cout << "Failed to load font file. " << endl;
    }
    
    sf::Text winText; 
    winText.setFont(font);
    winText.setString("You Won!");
    winText.setFillColor(sf::Color::Green);
    winText.setCharacterSize(50);
    winText.setPosition(200, 250);
    
    sf::Text loseText;
    loseText.setFont(font);
    loseText.setString("You Lost!");
    loseText.setFillColor(sf::Color::Red);
    loseText.setCharacterSize(50);
    loseText.setPosition(200, 250);
    
    sf::Text remainingP1; 
    remainingP1.setFont(font);
    remainingP1.setString("# of Bricks Remaining: ");
    remainingP1.setCharacterSize(15);
    remainingP1.setPosition(15,0);
    
    sf::Text remainingP2; 
    string remainingNumber{"50"}; // Initial remaining bricks display. 
    remainingP2.setFont(font);
    remainingP2.setString(remainingNumber);
    remainingP2.setCharacterSize(15);
    remainingP2.setPosition(175, 0);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (isPlaying){

        	
		// Ball movement animations. 
		updateBall(xChangeRate, yChangeRate, xPosition, yPosition);
		
		// Paddle collision detection. ===============================================================
		
		// Define ball collision points and their positions. 
		int ballTopX{xPosition + radius};
		int ballTopY{yPosition};
		int ballBottomY{yPosition + diameter};
		int ballBottomX{xPosition + radius};
		int ballRightX{xPosition + diameter};
		int ballRightY{yPosition + radius};
		int ballLeftX{xPosition}; 
		int ballLeftY{yPosition + radius};
		
		sf::Vector2f ballTopCollision(sf::Vector2f(ballTopX, ballTopY));
		sf::Vector2f ballBottomCollision(sf::Vector2f(ballBottomX, ballBottomY));
		sf::Vector2f ballRightCollision(sf::Vector2f(ballRightX, ballRightY));
		sf::Vector2f ballLeftCollision(sf::Vector2f(ballLeftX, ballLeftY));
		
		// Paddle collision detection ==================================================================
		
		bool paddleCollision = ballCollidedPaddle(ballTopCollision, ballBottomCollision, ballRightCollision, ballLeftCollision, paddle);
		
		if(paddleCollision){
			yChangeRate *= -1;
		}
	     
		// Edge collision detection ====================================================================
		
		bool edgeCollision = ballCollidedEdge(xPosition, yPosition, diameter); 
		
		if (edgeCollision){
			if (yPosition <= 0){ // If ball touches top wall (user wins).
				setWin(isPlaying, hasWon);
			} else if (yPosition >= 600 - diameter){ // If ball touches bottom wall (user loses).  
				setLoss(isPlaying, hasLost); 
			} else { // If ball touches side edges (ball bounces off). 
				xChangeRate *= -1; 
			}
		}
	       
		// Set new ball position. 
		ball.setPosition(xPosition, yPosition);
		window.clear();
		
	       // Paddle Animations =============================================================================
	      
	       updatePaddle(paddleCurrentX, paddleCurrentY, paddleRightCurrentX, paddleInitialX, paddleInitialY, paddleLength, paddle);
	        
		// Draw on-screen elements ======================================================================
		
		window.draw(remainingP1);
		window.draw(remainingP2);
		window.draw(ball);
		window.draw(paddle);
		
		sf::sleep(sf::milliseconds(20));
		
		// Ball collision detection with brick wall. =====================================================
		
		int collidedBrickNum = ballCollidedWall(ballTopCollision, ballBottomCollision, ballRightCollision, ballLeftCollision, brick, yChangeRate, xChangeRate); 
		
		// Remove brick if collision is detected. 
		if (collidedBrickNum > 0){ 
			bricksRemaining -= 1; 
			removeBrick(brick, collidedBricks, collidedBrickNum); 
		}  	
	
		// Update remaining brick number display 
	        remainingNumber = to_string(bricksRemaining);
	        remainingP2.setString(remainingNumber);
	        
		// Draw non-collided bricks. 
		for(int index = 1; index <= 50; index++){
			if (collidedBricks[index] != 1){
				window.draw(brick[index]);
			}
		}
	
        } else if (hasWon) {
        	window.clear();
        	window.draw(winText);
        	window.display();
        
        } else if (hasLost) {
        	window.clear();
        	window.draw(loseText);
        	window.display();
        
        }
       
        // end of window.draw() calls.

        window.display();
       
    }
	
    return 0;
}

// Initialize Wall Function ==================================================================

void initializeWall(sf::RectangleShape (&brick)[51]){
	// Declare and initialize counters for brick wall. 
         
         const int brickLength{60};
   	 const int brickHeight{25};
   	 int brickPositionX{0};
   	 int brickPositionY{50};
   	 int brickRow{1};
   	 int xMultiplier{0};
   	 
	// For loop draws each brick with modified positioning for each iteration. 
        for (int rectangleCount = 1; rectangleCount <= 50; rectangleCount++){ 
        	// Every 10 bricks, the y-coordinate is shifted down a row and the xMultiplier is reset. 
        	if (rectangleCount % 10 == 1.f){ 
        		brickPositionY += 25;
        		xMultiplier = 0;
        		brickRow += 1;
        	}
        	
        	brickPositionX = 60*xMultiplier; // Moves each consecutive brick over by 60 units. 

		// Set attributes for each brick. 
		brick[rectangleCount].setSize(sf::Vector2f(brickLength, brickHeight));
		brick[rectangleCount].setPosition(brickPositionX, brickPositionY);
		brick[rectangleCount].setFillColor(sf::Color::Magenta);
		brick[rectangleCount].setOutlineThickness(1.f);
		brick[rectangleCount].setOutlineColor(sf::Color::White);
		
		if(brickRow == 1){
			brick[rectangleCount].setFillColor(sf::Color::Magenta);
		} else if (brickRow == 2){
			brick[rectangleCount].setFillColor(sf::Color::Red);
		} else if (brickRow == 3){
			brick[rectangleCount].setFillColor(sf::Color::Yellow);
		} else if (brickRow == 4){
			brick[rectangleCount].setFillColor(sf::Color::Green);
		} else if (brickRow == 5){
			brick[rectangleCount].setFillColor(sf::Color::Blue);
		}
		
        	xMultiplier += 1;
        }
}

// Initialize Paddle Function ======================================================================

void initializePaddle(const int paddleLength, int paddleInitialX, int paddleInitialY, sf::RectangleShape &paddle){
    const int paddleHeight{25};
    paddle.setSize(sf::Vector2f(paddleLength, paddleHeight));
    paddle.setPosition(paddleInitialX, paddleInitialY); // center the paddle position. 
}

// Update Paddle Function  =========================================================================

void updatePaddle(int &paddleCurrentX, int &paddleCurrentY, int &paddleRightCurrentX, int paddleInitialX, int paddleInitialY, const int paddleLength, sf::RectangleShape &paddle){
	
	// Declare and initialize paddle movement constants and position boundaries. 
	 const int moveAmtX{6};
	 const int moveAmtY{0};
	 const int illegalLeftX{0};
	 const int illegalRightX{480};
    	 
    	 // Detect keyboard press. 
	 if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { // Move the paddle to the right when the right key is pressed.
	  		if (paddleCurrentX <= illegalRightX){ 
	  			paddle.move(moveAmtX, moveAmtY);
	  			paddleCurrentX += moveAmtX;
	  			paddleRightCurrentX += moveAmtX;  
	  		}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { // Move the paddle to the left when the left key is pressed. 
	   		if (paddleCurrentX >= illegalLeftX){ 
	   			paddle.move(-moveAmtX, moveAmtY);
	   			paddleCurrentX -= moveAmtX;
	   			paddleRightCurrentX -= moveAmtX; 
	   		}
		}
	       
}

// Update Ball Function ============================================================================

void updateBall(int xChangeRate, int yChangeRate, int &xPosition, int &yPosition){
	xPosition += xChangeRate;
	yPosition += yChangeRate;
}

// Ball Collision With Brick Wall Detection Function ===============================================

int ballCollidedWall(sf::Vector2f ballTopCollision, sf::Vector2f ballBottomCollision, sf::Vector2f ballRightCollision, sf::Vector2f ballLeftCollision, sf::RectangleShape brick[51], int &yChangeRate, int &xChangeRate){
	/*  Returned index = collided with bottom of brick with top collision point.  
	    Returned 0 = no collision. 
	    Returned -1 = collided with side of brick. 
	    Returned -2 = collided with bottom of brick with bottom collision point. 
	*/
	for (int index = 1; index <= 50; index++){ 
		sf::FloatRect brickCollisionBounds = brick[index].getGlobalBounds();
			
		if (brickCollisionBounds.contains(ballTopCollision)){
			yChangeRate *= -1; 
			return index; 
	
		} else if (brickCollisionBounds.contains(ballBottomCollision)){
			yChangeRate *= -1; 
			return -2; 

		} else if  (brickCollisionBounds.contains(ballRightCollision) || brickCollisionBounds.contains(ballLeftCollision)){
			xChangeRate *= -1; 
			return -1; 
		} 
	}
	return 0; 
}

// Ball Collision With Edge Detection Function ====================================================

bool ballCollidedEdge(int xPosition, int yPosition, int diameter){
	if (xPosition >= (600 - diameter) || xPosition <= 0){
		return true; 
	};
		
	// If ball touches top wall (user wins). 
		
	if (yPosition <= 0){
		return true;
	}
	// If ball touches bottom wall (user loses). 
		
	if (yPosition >= 600 - diameter){
		return true; 
	};
	return false; 
}

// Ball Collision With Paddle Detection Function ===================================================

bool ballCollidedPaddle(sf::Vector2f ballTopCollision, sf::Vector2f ballBottomCollision, sf::Vector2f ballRightCollision, sf::Vector2f ballLeftCollision, sf::RectangleShape paddle){
	sf::FloatRect paddleCollisionBounds = paddle.getGlobalBounds();
	       
	if (paddleCollisionBounds.contains(ballBottomCollision)){
		return true;
	} else if (paddleCollisionBounds.contains(ballTopCollision)){
	      	return true;
	} else if (paddleCollisionBounds.contains(ballRightCollision)){
	      	return true;
	} else if (paddleCollisionBounds.contains(ballLeftCollision)){
	      	return true;
	}
	return false; 
}

// Remove Brick Function ===========================================================================

void removeBrick(sf::RectangleShape (&brick)[51], int (&collidedBricks)[51], int collidedBrickNum){
	collidedBricks[collidedBrickNum] = 1;
	brick[collidedBrickNum].setSize(sf::Vector2f(0,0));
}

// Set Win Function ================================================================================

void setWin(bool &isPlaying, bool &hasWon){
	isPlaying = false; 
	hasWon = true;
}

// Set Loss Function ===============================================================================

void setLoss(bool &isPlaying, bool &hasLost){
	isPlaying = false; 
	hasLost = true; 
}

