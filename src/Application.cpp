#include "Application.h"

#include <SFML/OpenGL.hpp>

#include <iostream>
#include <sstream>
#include <cmath>

#include "Background.h"
#include "ui/UI_CameraInfo.h"

#include "glm/gtc/quaternion.hpp"

Application::Application()
	: m_window(sf::VideoMode::getDesktopMode(), "Application", sf::Style::Fullscreen ), 
	  m_entitymanager(this), m_entitymanager_ui(this), m_entitymanager_background(this), m_selectedEntityId(0)
{ std::cout << "Opened context." << std::endl; }

std::string textDump[] = {"As seen on TV!","Awesome!","100% pure!","May contain nuts!","Better than Prey!","More polygons!","Sexy!","Limited edition!","Flashing letters!","Made by Notch!","It's here!","Best in class!","It's finished!","Kind of dragon free!","Excitement!","More than 500 sold!","One of a kind!","Heaps of hits on YouTube!","Indev!","Spiders everywhere!","Check it out!","Holy cow, man!","It's a game!","Made in Sweden!","Uses LWJGL!","Reticulating splines!","Minecraft!","Yaaay!","Singleplayer!","Keyboard compatible!","Undocumented!","Ingots!","Exploding creepers!","That's no moon!","l33t!","Create!","Survive!","Dungeon!","Exclusive!","The bee's knees!","Down with O.P.P.!","Closed source!","Classy!","Wow!","Not on steam!","Oh man!","Awesome community!","Pixels!","Teetsuuuuoooo!","Kaaneeeedaaaa!","Now with difficulty!","Enhanced!","90% bug free!","Pretty!","12 herbs and spices!","Fat free!","Absolutely no memes!","Free dental!","Ask your doctor!","Minors welcome!","Cloud computing!","Legal in Finland!","Hard to label!","Technically good!","Bringing home the bacon!","Indie!","GOTY!","Ceci n'est pas une title screen!","Euclidian!","Now in 3D!","Inspirational!","Herregud!","Complex cellular automata!","Yes, sir!","Played by cowboys!","OpenGL 1.2!","Thousands of colors!","Try it!","Age of Wonders is better!","Try the mushroom stew!","Sensational!","Hot tamale, hot hot tamale!","Play him off, keyboard cat!","Guaranteed!","Macroscopic!","Bring it on!","Random splash!","Call your mother!","Monster infighting!","Loved by millions!","Ultimate edition!","Freaky!","You've got a brand new key!","Water proof!","Uninflammable!","Whoa, dude!","All inclusive!","Tell your friends!","NP is not in P!","Notch <3 ez!","Music by C418!","Livestreamed!","Haunted!","Polynomial!","Terrestrial!","All is full of love!","Full of stars!","Scientific!","Cooler than Spock!","Collaborate and listen!","Never dig down!","Take frequent breaks!","Not linear!","Han shot first!","Nice to meet you!","Buckets of lava!","Ride the pig!","Larger than Earth!","sqrt(-1) love you!","Phobos anomaly!","Punching wood!","Falling off cliffs!","0% sugar!","150% hyperbole!","Synecdoche!","Let's danec!","Seecret Friday update!","Reference implementation!","Lewd with two dudes with food!","Kiss the sky!","20 GOTO 10!","Verlet intregration!","Peter Griffin!","Do not distribute!","Cogito ergo sum!","4815162342 lines of code!","A skeleton popped out!","The Work of Notch!","The sum of its parts!","BTAF used to be good!","I miss ADOM!","umop-apisdn!","OICU812!","Bring me Ray Cokes!","Finger-licking!","Thematic!","Pneumatic!","Sublime!","Octagonal!","Une baguette!","Gargamel plays it!","Rita is the new top dog!","SWM forever!","Representing Edsbyn!","Matt Damon!","Supercalifragilisticexpialidocious!","Consummate V's!","Cow Tools!","Double buffered!","Fan fiction!","Flaxkikare!","Jason! Jason! Jason!","Hotter than the sun!","Internet enabled!","Autonomous!","Engage!","Fantasy!","DRR! DRR! DRR!","Kick it root down!","Regional resources!","Woo, facepunch!","Woo, somethingawful!","Woo, /v/!","Woo, tigsource!","Woo, minecraftforum!","Woo, worldofminecraft!","Woo, reddit!","Woo, 2pp!","Google anlyticsed!","Now supports åäö!","Give us Gordon!","Tip your waiter!","Very fun!","12345 is a bad password!","Vote for net neutrality!","Lives in a pineapple under the sea!","MAP11 has two names!","Omnipotent!","Gasp!","...!","Bees, bees, bees, bees!","Jag känner en bot!","This text is hard to read if you play the game at the default resolution, but at 1080p it's fine!","Haha, LOL!","Hampsterdance!","Switches and ores!","Menger sponge!","idspispopd!","Eple (original edit)!","So fresh, so clean!","Slow acting portals!","Try the Nether!","Don't look directly at the bugs!","Oh, ok, Pigmen!","Finally with ladders!","Scary!","Play Minecraft, Watch Topgear, Get Pig!","Twittered about!","Jump up, jump up, and get down!","Joel is neat!","A riddle, wrapped in a mystery!","Huge tracts of land!","Welcome to your Doom!","Stay a while, stay forever!","Stay a while and listen!","Treatment for your rash!","Autological is!","Information wants to be free!","Almost never is an interesting concept!","Lots of truthiness!","The creeper is a spy!","Turing complete!","It's groundbreaking!","Let our battle's begin!","The sky is the limit!","Jeb has amazing hair!","Casual gaming!","Undefeated!","Kinda like Lemmings!","Follow the train, CJ!","Leveraging synergy!","This message will never appear on the splash screen, isn't that weird?","DungeonQuest is unfair!","110813!","90210!","Check out the far lands!","Tyrion would love it!","Also try VVVVVV!","Also try Super Meat Boy!","Also try Terraria!","Also try Mount And Blade!","Also try Project Zomboid!","Also try World of Goo!","Also try Limbo!","Also try Pixeljunk Shooter!","Also try Braid!","That's super!","Bread is pain!","Read more books!","Khaaaaaaaaan!","Less addictive than TV Tropes!","More addictive than lemonade!","Bigger than a bread box!","Millions of peaches!","Fnord!","This is my true form!","Totally forgot about Dre!","Don't bother with the clones!","Pumpkinhead!","Hobo humping slobo babe!","Made by Jeb!","Has an ending!","Finally complete!","Feature packed!","Boots with the fur!","Stop, hammertime!","Testificates!","Conventional!","Homeomorphic to a 3-sphere!","Doesn't avoid double negatives!","Place ALL the blocks!","Does barrel rolls!","Meeting expectations!","PC gaming since 1873!","Ghoughpteighbteau tchoghs!","Déjà vu!","Déjà vu!","Got your nose!","Haley loves Elan!","Afraid of the big, black bat!","Doesn't use the U-word!","Child's play!","See you next Friday or so!","From the streets of Södermalm!","150 bpm for 400000 minutes!","Technologic!","Funk soul brother!","Pumpa kungen!","My life for Aiur!","Lennart lennart = new Lennart();","I see your vocabulary has improved!","Who put it there?","You can't explain that!","if not ok then return end","SOPA means LOSER in Swedish"};

void Application::Initialize() {
	m_window.setVerticalSyncEnabled(false);

	// Fetch some default assets
	std::cout << "Loading default assets..." << std::endl;
	m_debugFont.loadFromFile("../media/font_debug.ttf");
	m_textShader_default.loadFromFile("../media/text_default.frag", sf::Shader::Fragment);

	// Set up OpenGL environment
	std::cout << "Fiddling with OpenGL..." << std::endl;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	// Configure the viewport (the same size as the window)
	// TODO: Take care of relativity: new sizes; text-clipping?
	glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);

	// Assign picking buffer
	glSelectBuffer( PICK_BUFFER_SIZE, m_pickBuffer );

	m_entitymanager_background.AddEntity( new Background() );

	for( int i = 0; i != 100; ++i ) {
		String3D *str = dynamic_cast<String3D*>(m_entitymanager.AddEntity( new String3D ));
		str->SetString(textDump[i], &m_SDFontManager.Fetch("../media/sdf1.txt", "../media/sdf1.png"), &m_textShader_default);
		str->m_position.x = rand()%500-250;
		str->m_position.y = rand()%500-250;
		str->m_position.z = rand()%500-250;
		str->m_rotation.x = rand()%360;
		str->m_rotation.y = rand()%360;
		str->m_rotation.z = rand()%360;
	}

	m_entitymanager_ui.AddEntity( new UI_CameraInfo() );
}

void Application::Run() {
	std::cout << "Entering core logic..." << std::endl;
	while (m_window.isOpen()) {
		Logic();
		Draw();
	}
}

void Application::Logic() {
	// Handle time
	m_delta = m_deltaClock.restart().asSeconds();
	m_elapsedTime += m_delta;

	// Check for events
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();

		// TODO: Remove. Use Alt-F4 for quitting instead; internal GUI quit logic
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape ) {
			m_window.close();
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space ) {
			m_firstPersonCamera.AddNewKeyframe( Keyframe( m_firstPersonCamera.m_position, glm::quat_cast(m_firstPersonCamera.m_rotationMatrix), SelectedEntityId() ));
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left ) {
			m_firstPersonCamera.PrevKeyframe();
			m_selectedEntityId = m_firstPersonCamera.overriddenSelectionID;
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right ) {
			m_firstPersonCamera.NextKeyframe();
			m_selectedEntityId = m_firstPersonCamera.overriddenSelectionID;
		}

		if (event.type == sf::Event::MouseWheelMoved) {
			m_firstPersonCamera.m_mouseWheelDelta = event.mouseWheel.delta;
		}

		if (event.type == sf::Event::Resized)
			glViewport(0, 0, event.size.width, event.size.height);

		m_entitymanager_ui.HandleSFMLEvent( event );
		m_desktop.HandleEvent(event);
	}

	// Camera updates
	m_firstPersonCamera.Update(m_delta, *this);

	// Other logic
	m_desktop.Update(m_delta);

	m_entitymanager_background.UpdateEntities( m_delta );
	m_entitymanager_ui.UpdateEntities( m_delta );
	m_entitymanager.UpdateEntities( m_delta );

	m_entitymanager_ui.DoDeletions();
	m_entitymanager.DoDeletions();
	m_entitymanager_background.DoDeletions();
}

void Application::Draw() {
	m_window.pushGLStates();
		m_window.clear(); // Could also draw 2D background here
		m_entitymanager_background.DrawEntities();
	m_window.popGLStates();

	glRenderMode( GL_SELECT );
		Draw_OpenGL(true);
	int nHits = glRenderMode( GL_RENDER );
		Draw_OpenGL(false);

	if(nHits > 0) {
		m_entitymanager.GetEntityWithId(m_pickBuffer[3])->MouseOver();
		if (sf::Mouse::isButtonPressed( sf::Mouse::Left)) {
			m_selectedEntityId = m_pickBuffer[3];
		}
	} else {
		//if (sf::Mouse::isButtonPressed( sf::Mouse::Left) )
		//	m_selectedEntityId = 0;
	}

	m_window.pushGLStates();

		// SFML DRAWING HERE
		m_entitymanager_ui.DrawEntities();

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		m_window.resetGLStates();
		m_sfgui.Display( m_window );
		glPopAttrib();

		DrawFPS();

	m_window.popGLStates();

	m_window.display();
}

void Application::Draw_OpenGL(bool selection) {
	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if( selection ) {
		int viewport[4] = {0, 0, m_window.getSize().x, m_window.getSize().y};
		gluPickMatrix( (double)sf::Mouse::getPosition().x, (double)m_window.getSize().y-(double)sf::Mouse::getPosition().y, PICK_TOL, PICK_TOL, viewport );
	}

	GLfloat ratio = static_cast<float>(m_window.getSize().x) / m_window.getSize().y;
	glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

	// Blank all names before draws
	glInitNames();
	glPushName(0xFFFFFFFF);

	// Begin camera-space rendering
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// CAMERA TRANSFORMS HERE
	m_firstPersonCamera.Transform();

	glClear(GL_DEPTH_BUFFER_BIT);

	// OPENGL DRAWING HERE
	m_entitymanager.DrawEntities();
}

void Application::DrawFPS() {
	std::ostringstream oss;
	oss << 1.0f/m_delta << " FPS";
	sf::Text fps(oss.str(), m_debugFont, 20U );
	m_window.draw(fps);
}

const sf::Vector2u &Application::GetSize() {
	return m_window.getSize();
}
