//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "spotLight.h"
#include "cube.h"
#include "curve.h"
#include "Sphere2.h"
#include "stb_image.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
//void bed(Shader& lightingShader, glm::mat4 alTogether, Cube& cube);

void Floor(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void FWall(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void Table(Cube &cube,Shader& lightingShader, glm::mat4 alTogether);
//void Sofa(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void DiningTable(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void Tool(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void Chair(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void Bookself(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
//void TV_Trolly(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void TV(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void Wardrobe(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void Shokez(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void Show(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void Door(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void bed(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void ladder(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
//void Ladder(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);//Main one

//void bus(Cube& cube, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void bus(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Cube& cube_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void Wheel(Curve& curve_cyl, Curve& curve_circle, Shader& lightingShader, glm::mat4 alTogether);
void Wheel_hollow(Curve& curve_cyl, Curve& curve_hollow_cyl, Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void terminal(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void lobby(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void seat(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void windmill_blades(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void windmill_body(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void road(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void make_boat(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Curve& tree, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void make_tree(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Curve& tree, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void sun_rotate(Sphere2& sphere, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);



glm::mat4 transform(float tr_x, float tr_y, float tr_z, float rot_x, float rot_y, float rot_z, float scal_x, float scal_y, float scal_z) {
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tr_x, tr_y, tr_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians( rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians( rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scal_x, scal_y, scal_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    return model;
}



// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
unsigned int ch_wood_tex;
unsigned int almari_tex;
unsigned int cushion_tex;
unsigned int black_tex;


unsigned int bus_side_right_tex;
unsigned int bus_side_left_tex;
unsigned int bus_front_tex;
unsigned int bus_back_tex;
unsigned int road_tex;
unsigned int stone_tex;
unsigned int sq_tile_tex;
//unsigned int road_tex;
unsigned int sun_tex;



Camera camera(glm::vec3(0.0f, 3.1f, 13.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);

// lights
// positions of the point lights
glm::vec3 LightPositions[] = {
    glm::vec3(35.0, 20.0, 35.0), //directional light1 - white
    glm::vec3(-5.12181 ,4.92 ,13.1597), //spot light - red
    glm::vec3(-6.75269 ,4.92 ,2.44401), //point light1 - green
    glm::vec3(8 ,14.0 ,-4), //point light2 - blue
    glm::vec3(4.36687 ,14.0 ,5.62477 ), //point light3 - yellow
    glm::vec3(-35.0, 20.0, -35.0) //directional light2 - grey
};
DirLight dirlight1(
    LightPositions[0].x, LightPositions[0].y, LightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 0.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
SpotLight spotlight(

    LightPositions[1].x, LightPositions[1].y, LightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight1(

    LightPositions[2].x, LightPositions[2].y, LightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    LightPositions[3].x, LightPositions[3].y, LightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    LightPositions[4].x, LightPositions[4].y, LightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.2f, 1.2f, 1.2f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
DirLight dirlight2(
    LightPositions[5].x, LightPositions[5].y, LightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 0.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);



// light settings
bool pointLightOn1 = true;
bool pointLightOn2 = true;
bool pointLightOn3 = true;
bool dirLightOn1 = true;
bool dirLightOn2 = true;
bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool emissionToggle = true;

bool lobby_door_fl = false;
float lobby_door_tx = 1.0f;
bool bus_door_fl = false;
float bus_door_angle = 0.0f;
float wdz = 0;
bool bus_move_fl1 = false;
bool bus_move_fl2 = false;
float bus_move_tx1 = 1.0f;
float bus_move_tx2 = 1.0f;
float sun_rotate_y = 0.5f;
float sun_rotate_axis = 0.5f;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

Cube* demo;

vector<float>wheel_lid_vertices = {
    0.1750, 1.9850, 5.1000,
    -0.3050, 1.9650, 5.1000,
    -0.5800, 1.9650, 5.1000,
};

vector<float>wheel_vertices = {
    -0.7300, 2.3200, 5.1000,
    -0.7400, 2.1250, 5.1000,
    -0.7350, 1.9250, 5.1000,
    -0.7250, 1.6700, 5.1000,
    -0.7250, 1.4700, 5.1000,
    -0.7250, 1.2600, 5.1000,
    -0.7200, 1.0350, 5.1000
};

vector<float>hollow_cyllinder_vertices = 
//{
//    -0.2550, 1.6600, 5.1000,
//    -0.8050, 0.9150, 5.1000
//};
{
    -0.4900, 1.9100, 5.1000,
    -1.5750, 1.7800, 5.1000,
};

vector<float>tree_vertices = {
    -0.3800, 1.8200, 5.1000,
    -0.4600, -0.2750, 5.1000,
    -0.9950, 1.4450, 5.1000,
    -1.0400, 0.1600, 5.1000,
    -1.5800, 0.8500, 5.1000,
};

vector<float>share_tree_vertices = {
    -1.5100, 1.0500, 5.1000,
    -1.0150, 1.5850, 5.1000,
    -0.9450, 0.4250, 5.1000,
    -0.2300, 2.2150, 5.1000,
    -0.1250, -0.2200, 5.1000,
};

vector<float>apple_shape_vertices =
{
    -0.7250, 2.0650, 5.1000,
    -1.3300, 1.3600, 5.1000,
    -0.7450, 0.5650, 5.1000,
};

vector<float>baloon_shape_vertices =
{
    -0.0550, 2.3550, 5.1000,
    -1.1900, 1.3050, 5.1000,
    -0.0500, 0.1700, 5.1000,
};

vector<float>mushroom_shape_vertices =
{
    -1.5900, 1.9000, 5.1000,
    -1.1350, 1.6850, 5.1000,
    -0.7650, 1.6600, 5.1000,
    -0.4750, 1.7000, 5.1000,
    -0.2850, 1.7000, 5.1000,
    -0.0800, 1.6950, 5.1000,
    0.1800, 1.6600, 5.1000,
    0.3450, 1.6600, 5.1000,
    0.5200, 1.7000, 5.1000,
    0.5850, 1.7250, 5.1000,
    0.8000, 1.8550, 5.1000,
    1.0300, 1.9850, 5.1000,
};



//Curve* wheel_pointer;
//Curve* wheel_lid_pointer;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    string diffuseMapPath = "container2.png";
    string specularMapPath = "container2_specular.png";
    string f_tiles = "stone_ground.jpg";
    string wood = "wood.png";
    string woods = "woods.jpg";
    string sofa = "sofa.jpg";
    string marbel = "marbel.jpg";
    string cushion = "cushion.jpg";
    string plywood = "plywood.jpg";
    string ch_wood = "ch_wood.jpg";
    string chair = "chair.jpg";
    string wall = "wall.jpg";
    string walls = "walls.jpg";
    string black = "black.jpg";
    string news = "news.jpg";
    string city = "city.jpg";
    string almari = "almari.jpg";
    string door = "door.jpg";

    string bus_side_right = "bus-side-black-right.jpg";
    string  bus_side_left = "bus-side-black-left.jpg";
    string      bus_front = "bus-side-black-front.jpg";
    string       bus_back = "bus-side-black-back.jpg";
    string         road_o = "road.jpg";
    string         stone  = "stone_ground.jpg";
    string        sq_tile = "sq_tile.jpg";
    string        sunP = "sun.png";



    //bus_side_right_tex = loadTexture(bus_side_right.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //bus_side_left_tex = loadTexture(bus_side_left.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //bus_front_tex = loadTexture(bus_front.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //bus_back_tex = loadTexture(bus_back.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    



    //unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int wood_tex = loadTexture(wood.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int woods_tex = loadTexture(woods.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int sofa_tex = loadTexture(sofa.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int marbel_tex = loadTexture(marbel.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int plywood_tex = loadTexture(plywood.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int chair_tex = loadTexture(chair.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int wall_tex = loadTexture(wall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int floor_tiles_tex = loadTexture(f_tiles.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int walls_tex = loadTexture(walls.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int news_tex = loadTexture(news.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int city_tex = loadTexture(city.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    ch_wood_tex = loadTexture(ch_wood.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    almari_tex = loadTexture(almari.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    cushion_tex = loadTexture(cushion.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    black_tex = loadTexture(black.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int door_tex = loadTexture(door.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    road_tex = loadTexture(road_o.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    stone_tex = loadTexture(stone.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    sq_tile_tex = loadTexture(sq_tile.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    sun_tex = loadTexture(sunP.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);





    Cube cube = Cube(walls_tex, walls_tex, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    demo = &cube;
    Cube tiles_cube = Cube(floor_tiles_tex, floor_tiles_tex, 92.0f, 0.0f, 0.0f, 20.0f, 20.0f);
    Cube cube_cyl = Cube(cushion_tex, cushion_tex, 32.0, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube stone_cube = Cube(stone_tex, stone_tex, 92.0f, 0.0f, 0.0f, 3.0f, 3.0f);
    Cube sq_tile_cube= Cube(sq_tile_tex, sq_tile_tex, 92.0f, 0.0f, 0.0f, 8.0f, 8.0f);



    //Sphere2 sphere = Sphere2() ;
    //sphere.setDefaults();
    Sphere2  sphere= Sphere2(3.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, ch_wood_tex,ch_wood_tex, 0.0f, 0.0f, 20.0f, 20.0f);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    /*Curve wheel(wheel_vertices);*/
    //Curve wheel_hollow(hollow_cyllinder_vertices);

    //Curve wheel_lid(wheel_lid_vertices);
    //wheel_lid_pointer = &wheel_lid;
    
    Curve wheel(wheel_vertices, black_tex,black_tex, 1.0f);
    Curve wheel_hollow(hollow_cyllinder_vertices, black_tex, black_tex, 1.0f);
    Curve boat(tree_vertices, ch_wood_tex, ch_wood_tex, 1.0f);
    Curve tree(tree_vertices, sq_tile_tex, sq_tile_tex, 1.0f);

    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.435f, 0.675f, 0.949f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);
        
        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);
       
        dirlight1.setUpPointLight(lightingShaderWithTexture);
        spotlight.setUpPointLight(lightingShaderWithTexture);
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        dirlight2.setUpPointLight(lightingShaderWithTexture);
        //dirligh1.turnOff();
        //spotlight.turnOff();
        pointlight1.turnOff();
        //pointlight2.turnOff();
        //pointlight3.turnOff();
        //dirligh2.turnOff();





        

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.42, 0.11, 8.18f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix;


        //Floor
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-25.2967, -0.9099926, 0.6557));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-30.0f,0.0f,-30.0f));
        Floor(sq_tile_cube, lightingShaderWithTexture, translateMatrix);


        //model = transform(0.0f, 2.69f, -15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        ////bus(cube, lightingShaderWithTexture, model);
        //bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);






{

        //cube.setTextureProperty(diffMap, specMap, 32.0f);
        //cube.drawCubeWithTexture(lightingShaderWithTexture, model);
        
        ////Drawing Table
        //cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.065, 4.86783));
        //Table(cube, lightingShaderWithTexture, translateMatrix);
        
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
        //Ladder(cube, lightingShaderWithTexture, translateMatrix);

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.42, 0.11, 8.18f));
        //Tool(cube, lightingShaderWithTexture, translateMatrix);
        ////cube.setTextureProperty(woods_tex, woods_tex, 32.0f);
        ////translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0, 0.065, 4.86783));
        ////Table(cube, lightingShaderWithTexture, translateMatrix);

        //cube.setTextureProperty(marbel_tex, marbel_tex, 32.0f);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.10874, 0, 5.57778));
        //DiningTable(cube, lightingShaderWithTexture, translateMatrix);

        //////front
        //cube.setTextureProperty(chair_tex, chair_tex, 32.0f);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(6.57002, -0.08, -4.6));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
        //Chair(cube, lightingShaderWithTexture, translateMatrix* rotateYMatrix* scaleMatrix);

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(5.33, -0.08, -4.6));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
        //Chair(cube, lightingShaderWithTexture, translateMatrix* rotateYMatrix* scaleMatrix);
        //back
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(6.57002, -0.08, -8.03));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
        //Chair(cube, lightingShaderWithTexture, translateMatrix* scaleMatrix);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(5.33, -0.08, -8.03));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
        //Chair(cube, lightingShaderWithTexture, translateMatrix* scaleMatrix);

        //cube.setTextureProperty(walls_tex, walls_tex, 32.0f);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-25.2967, -0.9099926, 0.6557));
        //Floor(cube, lightingShaderWithTexture, translateMatrix);//-4.22f, -0.989999f, -5.28003f
        
        //front wall
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-11.7402, -0.83, 0.67));
        //FWall(cube, lightingShaderWithTexture, translateMatrix);

        //back wall
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-11.7402, -0.83, 18));
        //FWall(cube, lightingShaderWithTexture, translateMatrix);

        //left wall
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-11.7402, -0.83, 18));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //FWall(cube, lightingShaderWithTexture, translateMatrix * rotateYMatrix);

        ////Middle 
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 1, 0.4));
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.53005, -0.83, 18));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //FWall(cube, lightingShaderWithTexture, scaleMatrix * translateMatrix * rotateYMatrix);
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 1, 0.52));
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.53005, -0.83, 35.0));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //FWall(cube, lightingShaderWithTexture, scaleMatrix * translateMatrix * rotateYMatrix);
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 0.5, 1));
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.27, 4.35, 18.441));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //FWall(cube, lightingShaderWithTexture, scaleMatrix * translateMatrix * rotateYMatrix);

        //right wall
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(5.7402, -0.83, 18));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //FWall(cube, lightingShaderWithTexture, translateMatrix * rotateYMatrix);

        //Ceil
        //cube.setTextureProperty(wall_tex, wall_tex, 32.0f);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-25.2967, 5, 0.6557));
        //Floor(cube, lightingShaderWithTexture, translateMatrix);//-4.22f, -0.989999f, -5.28003f
        
        ////Floor
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-25.2967, -0.9099926, 0.6557));
        //Floor(tiles_cube, lightingShaderWithTexture, translateMatrix);
        
        //cube.setTextureProperty(news_tex, news_tex, 32.0f);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.15, 0.54, 0.89));
        //Show(cube, lightingShaderWithTexture,translateMatrix);

        //cube.setTextureProperty(city_tex, city_tex, 32.0f);
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-13.5602, 0.83, -3.16));
        //Show(cube, lightingShaderWithTexture, rotateYMatrix*translateMatrix);

        //cube.setTextureProperty(door_tex, door_tex, 32.0f);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.75, -0.83, 17.87f));
        //Door(cube, lightingShaderWithTexture, translateMatrix);

        //cube.setTextureProperty(black_tex, black_tex, 32.0f);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.49, -0.85, 0.87));
        //TV_Trolly(cube, lightingShaderWithTexture, translateMatrix);
        
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(4.51001, 0.0, -8.6501));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
        //Wardrobe(cube, lightingShaderWithTexture, rotateYMatrix* translateMatrix);

        //cube.setTextureProperty(plywood_tex, plywood_tex, 32.0f);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(4.49, -0.08, -5.77));
        //Shokez(cube, lightingShaderWithTexture, rotateYMatrix* translateMatrix);
        
        ////middle sofa
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.76, 0.059, -9.0));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.0f, 1.0f));//
        //cube.setTextureProperty(sofa_tex, sofa_tex, 32);
        //Sofa(cube, lightingShaderWithTexture, translateMatrix* scaleMatrix);//,translate_X, translate_Y, translate_Z
        
        ////Right Sofa
        //cube.setTextureProperty(sofa_tex, sofa_tex, 32);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.89776, -0.099, -3.64292));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.0f, 1.0f));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(283.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //Sofa(cube, lightingShaderWithTexture, rotateYMatrix* translateMatrix* scaleMatrix);//,translate_X, translate_Y, translate_Z

        ////Left Sofa
        //cube.setTextureProperty(sofa_tex, sofa_tex, 32);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.06396, 0.043, -5.37979));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.0f, 1.0f));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(77.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //Sofa(cube, lightingShaderWithTexture, translateMatrix* rotateYMatrix* scaleMatrix);//,translate_X, translate_Y, translate_Z

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-8.4501, -0.83, 13.8402));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.5f, 3.0f, 4.5f));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //bed(cube, lightingShaderWithTexture, translateMatrix* rotateYMatrix* scaleMatrix);//,
}











        //light_for_materialistic_property
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        dirlight1.setUpPointLight(lightingShader);
        spotlight.setUpPointLight(lightingShader);
        pointlight1.setUpPointLight(lightingShader);
        pointlight2.setUpPointLight(lightingShader);
        pointlight3.setUpPointLight(lightingShader);
        dirlight2.setUpPointLight(lightingShader);
        //dirlight1.turnOff();
        //spotlight.turnOff();
        pointlight1.turnOff();
        //pointlight2.turnOff();
        //pointlight3.turnOff();
        //dirligh2.turnOff();

        

        ////glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        //model = transform(-1.4, -.8, -4, 0, 0, 0, .15, 5.3, .15);
        //bus(cube, lightingShader, model);
        float sx = 0.5f;

//basic wheel
//model = transform(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
//Wheel(wheel, wheel_lid, lightingShader, model);
//
//hollow wheel
//model = transform(-2.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
//Wheel_hollow(wheel, wheel_hollow, cube,lightingShader, lightingShaderWithTexture, model);

        
        
        if (!bus_move_fl1 && bus_move_tx1 < 13)
        {
            bus_move_tx1 += .08;
        }
        if (bus_move_fl1 && bus_move_tx1 > -28)
        {
            bus_move_tx1 -= .08;
        }
        model = transform(0.0f + bus_move_tx1, 1.15f, 13.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        ////bus(cube, lightingShaderWithTexture, model);
        //lightingShaderWithTexture.setMat4("view", view*model);
        //lightingShader.setMat4("view", view*model);
        bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);

        if (bus_move_fl2 && bus_move_tx2 < 13)
        {
            bus_move_tx2 += .08;
        }
        if (!bus_move_fl2 && bus_move_tx2 > -28)
        {
            bus_move_tx2 -= .08;
        }
        model = transform(14.0f + bus_move_tx2, 1.15f, 25.3f, 0.0f, 180.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);



        //static bus 1
        model = transform(0.0f, 1.15f, 0.0f, 0.0f, 90.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        //bus(cube, lightingShaderWithTexture, model);
        bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);
        //static bus 2
        model = transform(15.0f, 1.15f, -5.0f, 0.0f, -90.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        //bus(cube, lightingShaderWithTexture, model);
        bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);



        model = transform(0.0f, 0.0f, -29.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        terminal(cube, lightingShader, lightingShaderWithTexture, model);



        model = transform(-28.0f, 0.0f, -25.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        //lightingShaderWithTexture.setMat4("view", view* transform(+28.0f, 0.0f, +22.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
        //lightingShader.setMat4("view", view* transform(+28.0f, 0.0f, +22.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
        lobby(cube, lightingShader, lightingShaderWithTexture, model);

        //model = transform(0.0f, 5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        //seat(cube, lightingShader, lightingShaderWithTexture, model);

//testing pos
//model = transform(0.0f, 1.15f, 0.0f, 0.0f, 90.0f, 0.0f, 1.0f, 1.0f, 1.0f);
////bus(cube, lightingShaderWithTexture, model);
//bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);

        
        //for (int i = 0; i < 4; i++)
        //{
            ////windmill blades 
            //model = transform(0.0f, 1.15 + 8, 0.0f, 0.0f, 0.0f, 0.0f + wdz, .25, 1.0f, 1.0f);
            //windmill_blades(cube, lightingShader, lightingShaderWithTexture, model);            
            ////windmill blades 
            //model = transform(0.0f, 1.15 + 8, 0.0f, 0.0f, 0.0f, 90.0f + wdz, .25, 1.0f, 1.0f);
            //windmill_blades(cube, lightingShader, lightingShaderWithTexture, model);
            ////windmill blades 
            //model = transform(0.0f, 1.15 + 8, 0.0f, 0.0f, 0.0f, 180.0f + wdz, .25, 1.0f, 1.0f);
            //windmill_blades(cube, lightingShader, lightingShaderWithTexture, model);
            ////windmill blades 
            //model = transform(0.0f, 1.15 + 8, 0.0f, 0.0f, 0.0f, 270.0f + wdz, .25, 1.0f, 1.0f);
            //windmill_blades(cube, lightingShader, lightingShaderWithTexture, model);
            //wdz += .25;
        //}

        model = transform(-23.0f, 1.15 - 1.15, 02.0f, 0.0f, 0.0f, 0.0f, 1, 1.0f, 1.0f);
        windmill_body(cube,wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);


        model = transform(-30.0f, 0.0f, 12.0f, 0.0f, 0.0f, 0.0f, 60.0f, 1, 1);
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        road(cube, lightingShader, lightingShaderWithTexture, model);

        

        //model = transform(10.0f, 1.15 + 10, 02.0f, 0.0f, 0.0f, 0.0f, 1, 1.0f, 1.0f);
        //make_boat(cube, wheel, wheel_hollow, boat, lightingShader, lightingShaderWithTexture, model);


        //model = transform(10.0f, 1.15 +10, 02.0f, 0.0f, 0.0f, 0.0f, 1, 1.0f, 1.0f);
        //make_tree(cube, wheel, wheel_hollow, tree, lightingShader, lightingShaderWithTexture, model);

//        //sphere
//        float px = -35 * glm::cos(glm::radians(sun_rotate_y)) ;
//        float pz = -35 * glm::sin(glm::radians(sun_rotate_y));
////        
//        //own axis rotation
//        model = transform(10.0f, 10, 02.0f, 0.0f, 0.0f+ sun_rotate_axis, 0.0f, 1, 1.0f, 1.0f);
//        sphere.setRadius(5);
//        sphere.setTextureProperty(sun_tex, sun_tex, 1.0f);
//        sphere.drawSphereWithTexture(lightingShaderWithTexture, model);
//        sun_rotate_axis += .25;
////
//        //around y rotate
//        model = transform(0,0,0, 0.0f, 0.0f, 0.0f, 1, 1.0f, 1.0f) * model;
//        model = transform(px, 12, pz, 0.0f, 0.0f + sun_rotate_y, 0.0f, 1, 1.0f, 1.0f) * model ;
//        sphere.setRadius(5);
//        sphere.setTextureProperty(sun_tex, sun_tex, 1.0f);
//        sphere.drawSphereWithTexture(lightingShaderWithTexture, model);
//        sun_rotate_y += .25;


        model = transform(0,0,0, 0.0f, 0.0f, 0.0f, 1, 1.0f, 1.0f) ;
        sun_rotate(sphere, lightingShader, lightingShaderWithTexture, model);

        //`


{
        //glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.065, 4.86783));
        //
        ////Table(cube, lightingShader, translateMatrix);

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.22, 0.47, 0.87));
        //TV(cube, lightingShader, translateMatrix);
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.69, -0.82, 14.6902));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 3.0f, 2.5f));
        ////rotateYMatrix = glm::rotate(identityMatrix, glm::radians(77.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //Bookself(cube, lightingShader, lightingShaderWithTexture, translateMatrix* scaleMatrix);//,translate_X, translate_Y, translate_Z
       
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.36, -0.82, 19.69));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 3.0f, 2.5f));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //Bookself(cube, lightingShader, lightingShaderWithTexture, translateMatrix* rotateYMatrix* scaleMatrix);
}







        // also draw the lamp object(s)
        
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        glm::vec3 LightColor[] = {
        glm::vec3(0.8f, 0.8f, 0.8f),// White - Dir
        glm::vec3(1.0f,  0.0f,  0.0f),//Red - Spot Light
        glm::vec3(0.0f,  1.0f,  0.0f),//Green - Point Light 1
        glm::vec3(0.0f,  0.0f,  1.0f),//Blue - Point Light 2
        glm::vec3(1.0f,  1.0f,  0.0f),//Yellow - Point Light 3
        glm::vec3(0.612f, 0.612f, 0.612f)// Grey - Dirlight2
        };
        
        for (unsigned int i = 0; i < 6 ; i++)
        {
            model = glm::mat4(1.0f);
            //LightPositions[i].y = translate_Y;
            model = glm::translate(model, LightPositions[i]);
            model = glm::scale(model, glm::vec3(0.25f)); // Make it a smaller cube
            cube.drawCube(ourShader, model, LightColor[i].x, LightColor[i].y, LightColor[i].z);
        }
        
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    cout << translate_X << ", " << translate_Y << ", " <<translate_Z << endl;
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }

    /*if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }*/
    //if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (dirLightOn1)
        {
            dirlight1.turnOff();
            dirLightOn1 = !dirLightOn1;
        }
        else
        {
            dirlight1.turnOn();
            dirLightOn1 = !dirLightOn1;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn1)
        {
            pointlight1.turnOff();
            pointLightOn1 = !pointLightOn1;
        }
        else
        {
            pointlight1.turnOn();
            pointLightOn1 = !pointLightOn1;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (pointLightOn2)
        {
            pointlight2.turnOff();
            pointLightOn2 = !pointLightOn2;
        }
        else
        {
            pointlight2.turnOn();
            pointLightOn2 = !pointLightOn2;
        }
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (spotLightOn)
        {
            spotlight.turnOff();
            spotLightOn = !spotLightOn;
        }
        else
        {
            spotlight.turnOn();
            spotLightOn = !spotLightOn;
        }
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            if (pointLightOn1)
                pointlight1.turnAmbientOff();
            if (pointLightOn2)
                pointlight2.turnAmbientOff();
            if (pointLightOn3)
                pointlight3.turnAmbientOff();
            if (spotLightOn)
                spotlight.turnAmbientOff();
            if (dirLightOn1)
                dirlight1.turnAmbientOff();
            if (dirLightOn2)
                dirlight2.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            if (pointLightOn1)
                pointlight1.turnAmbientOn();
            if (pointLightOn2)
                pointlight2.turnAmbientOn();
            if (pointLightOn3)
                pointlight3.turnAmbientOn();
            if (spotLightOn)
                spotlight.turnAmbientOn();
            if (dirLightOn1)
                dirlight1.turnAmbientOn();
            if (dirLightOn2)
                dirlight1.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            if (pointLightOn1)
                pointlight1.turnDiffuseOff();
            if (pointLightOn2)
                pointlight2.turnDiffuseOff();
            if (pointLightOn3)
                pointlight3.turnDiffuseOff();
            if (dirLightOn1)
                dirlight1.turnDiffuseOff();
            if (dirLightOn2)
                dirlight2.turnDiffuseOff();
            if (spotLightOn)
                spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            if (pointLightOn1)
                pointlight1.turnDiffuseOn();
            if (pointLightOn2)
                pointlight2.turnDiffuseOn();
            if (pointLightOn3)
                pointlight3.turnDiffuseOn();
            if (dirLightOn1)
                dirlight1.turnDiffuseOn();
            if (dirLightOn2)
                dirlight2.turnDiffuseOn();
            if (spotLightOn)
                spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            if (pointLightOn1)
                pointlight1.turnSpecularOff();
            if (pointLightOn2)
                pointlight2.turnSpecularOff();
            if (pointLightOn3)
                pointlight3.turnSpecularOff();
            if (dirLightOn1)
                dirlight1.turnSpecularOff();
            if (dirLightOn2)
                dirlight2.turnSpecularOff();
            if (spotLightOn)
                spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            if (pointLightOn1)
                pointlight1.turnSpecularOn();
            if (pointLightOn2)
                pointlight2.turnSpecularOn();
            if (pointLightOn3)
                pointlight3.turnSpecularOn();
            if (dirLightOn1)
                dirlight1.turnSpecularOn();
            if (dirLightOn2)
                dirlight2.turnSpecularOn();
            if (spotLightOn)
                spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }
    if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
        if (pointLightOn3)
        {
            pointlight3.turnOff();
            pointLightOn3 = !pointLightOn3;
        }
        else
        {
            pointlight3.turnOn();
            pointLightOn3 = !pointLightOn3;
        }
    }
    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        if (dirLightOn2)
        {
            dirlight2.turnOff();
            dirLightOn2 = !dirLightOn2;
        }
        else
        {
            dirlight2.turnOn();
            dirLightOn2 = !dirLightOn2;
        }
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        bus_door_fl = !bus_door_fl;
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        lobby_door_fl = !lobby_door_fl;
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        bus_move_fl1 = !bus_move_fl1;
    }
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        bus_move_fl2 = !bus_move_fl2;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Wheel(Curve& curve_cyl, Curve& curve_circle, Shader& lightingShader, glm::mat4 alTogether)
{
    glm::mat4 model;
    
    //cyllinder
    model = transform(0, 0, 0, 0, 0, 0, 1, .5, 1);
    //wheel_pointer->draw(lightingShader, alTogether * model, glm::vec3(0.52f, 0.39f, 0.31f));
    curve_cyl.draw(lightingShader, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //front circle
    model = transform(0, -2.773, 0, 0, 0, 0, 1.25, 2, 1.25);
    //wheel_lid_pointer->draw(lightingShader, alTogether * model, glm::vec3(0.32f, 0.30f, 0.61f));
    curve_circle.draw(lightingShader, alTogether * model, glm::vec3(0.32f, 0.32f, 0.32f));

    //back circle
    model = transform(0, -3.61, 0, 0, 0, 0, 1.25, 2.1, 1.25);
    //wheel_lid_pointer->draw(lightingShader, alTogether * model, glm::vec3(0.32f, 0.30f, 0.61f));
    curve_circle.draw(lightingShader, alTogether * model, glm::vec3(0.32f, 0.32f, 0.32f));
}

void Wheel_hollow(Curve& curve_cyl, Curve& curve_hollow_cyl, Cube& cube_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;
    curve_hollow_cyl.setTextureProperty(black_tex,black_tex, 1.0f);

    //cyllinder
    model = transform(0, 0, 0, 0, 0, 0, 1, 1, 1);
    curve_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //hollow cyllinder front
    model = transform(0, 2.23, 0, 0, 0, 0, .465, .05, .465);
    curve_hollow_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(1.0f,0.0f,0.0f));

    //hollow cyllinder back
    model = transform(0, .945, 0, 0, 0, 0, .4575, .05, .4575);
    curve_hollow_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(1.0f, 0.0f, 0.0f));

    //wheel center cube1 for texture(front)
    model = transform(-0.4, 2.25, -0.4, 0, 0, 0, 0.8, .04, 0.8);
    cube_cyl.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
    cube_cyl.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //wheel center cube2 for texture(back)
    model = transform(-0.4, 1.07, -0.4, 0, 0, 0, 0.8, .04, 0.8);
    cube_cyl.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
    cube_cyl.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);


}

void make_tree(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Curve& tree, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    //tree
    model = transform(0, 0, 0, 180, 0, 0, 7, 2, 1.5);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

}

void make_boat(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Curve& tree, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    //boat
    model = transform(0, 0, 0, 180, 0, 0, 6, 1.5, 2.5);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //model = transform(0, 0, 0, 180, 0, 0, 12, 1.5, 3.5);
    //curve_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));


}

void sun_rotate(Sphere2& sphere, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    //sphere
    float px = 42.4 * glm::cos(glm::radians(sun_rotate_y));
    float pz = 42.4 * glm::sin(glm::radians(sun_rotate_y));
    
    ////testing for own axis rotation
    //model = transform(10.0f, 10, 2.0f, 0.0f, 0.0f + sun_rotate_axis, 0.0f, 1, 1.0f, 1.0f);
    //sphere.setRadius(5);
    //sphere.setTextureProperty(sun_tex, sun_tex, 1.0f);
    //sphere.drawSphereWithTexture(lightingShaderWithTexture, model);
    //sun_rotate_axis += .28;
    

    //own axis rotation
    model = transform(0,0,0, 0.0f, 0.0f + sun_rotate_axis, 0.0f, 1, 1.0f, 1.0f);
    sun_rotate_axis += .28;

    //around y rotate
    model = transform(px, 20, pz, 0.0f, 0.0f + sun_rotate_y, 0.0f, 1, 1.0f, 1.0f) * model;
    sphere.setRadius(5);
    sphere.setTextureProperty(sun_tex, sun_tex, 1.0f);
    sphere.drawSphereWithTexture(lightingShaderWithTexture, model);
    sun_rotate_y += .28;
}

void road(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    //road strip1
    model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, .05f, 15.0f);
    cube.setTextureProperty(road_tex, road_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
}

void windmill_body(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    ////cyllinder
    //model = transform(0, 0, 0, 0, 0, 0, 1, 2, 1);
    //curve_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));


    float ty = 12.0f;
    float tz = .65f;

    //hollow cyllinder for stand
    model = transform(0, -178, 0, 0, 0, 0, .465, .05 + 100, .465);
    //curve_hollow_cyl.setTextureProperty(almari_tex, almari_tex, 1.0f);
    curve_hollow_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(1.0f, 0.0f, 0.0f));

    //stand notch
    model = transform(-.25f, 1.0 + ty, -.2f, 0.0f, 0.0f, 0.0f, .5, .35f, .85f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);


    //model = transform(0, 40, 0, 0, 0, 0, .465, .05 + 15, .465);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    
    
    //windmill blades 
    model = transform(0.0f, 1.15 + ty, 0.0f + tz, 0.0f, 0.0f, 0.0f + wdz, .25, 1.0f, 1.0f);
    windmill_blades(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
    //windmill blades 
    model = transform(0.0f, 1.15 + ty, 0.0f + tz, 0.0f, 0.0f, 90.0f + wdz, .25, 1.0f, 1.0f);
    windmill_blades(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
    //windmill blades 
    model = transform(0.0f, 1.15 + ty, 0.0f + tz, 0.0f, 0.0f, 180.0f + wdz, .25, 1.0f, 1.0f);
    windmill_blades(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
    //windmill blades 
    model = transform(0.0f, 1.15 + ty, 0.0f + tz, 0.0f, 0.0f, 270.0f + wdz, .25, 1.0f, 1.0f);
    windmill_blades(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
    wdz += .25;
}

void windmill_blades(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;
    float f,t;

    f = 0, t = 0;
    for (int j = 0; j < 1; j++)
    {
        f = 0;
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        model = transform(0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, .3, 8.0f, .3f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        model = transform(5.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, .3f, 8.0f, 0.3f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

        for (int i = 0; i < 14; i++)
        {
            //rail l1
            model = transform(0.3f , 0.0f + (f), 0.0f, 0.0f, 0.0f, 0.0f, 4.7f, 0.4f, .1f);
            cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32.0f);
            cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
            f += .6;
        }
        //model = transform(0, 8, 0, 0, 0, 0, 3,3,3);
        //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

        //t += 6;
    }

}

void lobby(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    float p1 = 1.0f;
    float p2 = 2.5f;
    float p3 = 8.0f;
    float p4 = 2.5f;
    float p5 = 1.0f;
    float panel_y = 5.0f;
    float panel_z = 0.35f;

    float floor_x = p1 + p2 + p3 + p4 + p5;
    float floor_y = 0.35f;
    float floor_z = 5.0f;

    float front_x = 0.35f;
    float front_y = floor_y + 5.0f;
    float front_z = 5.0f;


    ////surface
    //model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, p1 + p2 + p3 + p4 + p5, floor_y, floor_z + panel_z);
    //cube.setTextureProperty(black_tex, black_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //surface l1
    model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20.0f, .35f, 20.0f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    
    //surface l2
    model = transform(0.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20.0f, .35f, 20.0f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //chairs
    float t = 20;
    float startx = -4;
    for (int i = 0; i < 4; i++)
    {
        model = transform(startx + t, 1.5f, 4.0f, 0.0f, 0.0f, 0.0f, .6f, .6f, .6f);
        seat(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
        t -= 2.5;
    }
    t = 20;
    for (int i = 0; i < 4; i++)
    {
        model = transform(startx + t, 1.5f, 7.5f, 0.0f, 0.0f, 0.0f, .6f, .6f, .6f);
        seat(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
        t -= 2.55;
    }
    t = 20;
    for (int i = 0; i < 4; i++)
    {
        model = transform(startx + t, 1.5f, 11.0f, 0.0f, 0.0f, 0.0f, .6f, .6f, .6f);
        seat(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
        t -= 2.55;
    }
    t = 20;
    for (int i = 0; i < 4; i++)
    {
        model = transform(startx + t, 1.5f, 14.5f, 0.0f, 0.0f, 0.0f, .6f, .6f, .6f);
        seat(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
        t -= 2.55;
    }


    //fence l1 back
    float f = 1;
    for (int i = 0; i < 19; i++)
    {
        //rail l1
        model = transform(0.0f + (f), .35, 0.0f, 0.0f, 0.0f, 0.0f, 0.35f, 8 - .35, 1.0f);
        cube.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        f += 1;
    }
    //fence l1 front half
    f = 1;
    for (int i = 0; i < 13; i++)
    {
        //rail l1
        model = transform(6.0f + (f), .35, 19.0f, 0.0f, 0.0f, 0.0f, 0.35f, 8 - .35, 1.0f);
        cube.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        f += 1;
    }

    //fence l1 right
    f = 0;
    for (int i = 0; i < 19; i++)
    {
        //rail l1
        model = transform(20-1, .35, 1.0f + (f), 0.0f, 0.0f, 0.0f, 1.0f, 8 - .35, 0.35f);
        cube.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        f += 1;
    }

    //fence l1 left
    f = 0;
    for (int i = 0; i < 19; i++)
    {
        //rail l1
        model = transform(.0, .35, 1.0f + (f), 0.0f, 0.0f, 0.0f, 1.0f, 8 - .35, 0.35f);
        cube.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        f += 1;
    }

    //lobby door
    
    if (lobby_door_fl &&  lobby_door_tx < 5.9)
    {
        lobby_door_tx += 0.02f;
    }
    if (!lobby_door_fl && lobby_door_tx > 0.0)
    {
        lobby_door_tx -= 0.02f;
    }
    model = transform(1 + lobby_door_tx, .35, 19.35, 0.0f, 0.0f, 0.0f, 6, 8 - .35, 0.35f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);



    //t = 20;
    //for (int i = 0; i < 4; i++)
    //{
    //    model = transform(-4.0f + t, 1.5f, 9.0f, 0.0f, 0.0f, 0.0f, .5f, .5f, .5f);
    //    seat(cube, lightingShader, lightingShaderWithTexture, model);
    //    t -= 2.2;
    //}
    //t = 20;
    //for (int i = 0; i < 4; i++)
    //{
    //    model = transform(-4.0f + t, 1.5f, 12.0f, 0.0f, 0.0f, 0.0f, .5f, .5f, .5f);
    //    seat(cube, lightingShader, lightingShaderWithTexture, model);
    //    t -= 2.2;
    //}


}

void seat(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    //chair base
    model = transform(0.0f, 0.0f, 0.0f, 00.0f, 0.0f, 0.0f, 3.0f, .5f, 2.50f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //chair back
    model = transform(0.0f, 0.0f, 0.0f, -15.0f, 0.0f, 0.0f, 3.0f, 3.250f, 0.5f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //chair left
    model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15.0f, 0.5f, 2.250f, 2.4f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //chair right
    model = transform(2.5f, 0.2f, 0.0f, 0.0f, 0.0f, -15.0f, 0.5f, 2.250-.1, 2.4f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //leg1
    model = transform(0.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, .35f, 2.0f, .35);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //leg2
    model = transform(2.65f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, .35f, 2.0f, .35f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //leg3
    model = transform(0.0f, -2.0f, 2.15f, 0.0f, 0.0f, 0.0f, .35, 2.0f, .35);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //leg4
    model = transform(2.65f, -2.0f, 2.15f, 0.0f, 0.0f, 0.0f, .35, 2.0f, .35);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);



}

void terminal(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    float p1 = 1.0f;
    float p2 = 2.5f;
    float p3 = 8.0f;
    float p4 = 2.5f;
    float p5 = 1.0f;
    float panel_y = 5.0f;
    float panel_z = 0.35f;

    float floor_x = p1 + p2 + p3 + p4 + p5;
    float floor_y = 0.35f;
    float floor_z = 5.0f;

    float front_x = 0.35f;
    float front_y = floor_y + 5.0f;
    float front_z = 5.0f;
    
    ////surface
    //model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, p1 + p2 + p3 + p4 + p5, floor_y, floor_z + panel_z);
    //cube.setTextureProperty(black_tex, black_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);


    //surface l1
    model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20.0f, .35f, 8.0f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //vertical surface l1
    model = transform(20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 9.0f, .35f, 24.0f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //surface l2
    model = transform(0.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20.0f, .35f, 8.0f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //vertical surface l2
    model = transform(20.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 9.0f, .35f, 24.0f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);


    //fence l1
    float f = 0;
    //fence back l1
    float t = 0;
    for (int j = 0; j< 5; j++)
    {
        f = 0;
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        model = transform(0.0f+t, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, .3, 8.0f, .3f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        model = transform(5.0f+t, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, .3f, 8.0f, 0.3f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

        for (int i = 0; i < 14; i++)
        {
            //rail l1
            model = transform(0.3f+t, 0.0f + (f), 0.0f, 0.0f, 0.0f, 0.0f, 4.7f, 0.4f, .1f);
            cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32.0f);
            cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
            f += .6;
        }
        t += 6;
    }

    //fence right l1
    glm::mat4 tr;
    tr = transform(29.0f, 0.0f, 0.6f, 0.0f, -90.0f, 0.0f, 1.0, 1.0f, 1.0f);
    t = 0;
    for (int j = 0; j < 4; j++)
    {
        f = 0;
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        model = transform(0.0f + t, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, .3, 8.0f, .3f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * tr * model);
        model = transform(5.0f + t, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, .3f, 8.0f, 0.3f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * tr * model);

        for (int i = 0; i < 14; i++)
        {
            //rail l1
            model = transform(0.3f + t, 0.0f + (f), 0.0f, 0.0f, 0.0f, 0.0f, 4.7f, 0.4f, .1f);
            cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32.0f);
            cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * tr * model);
            f += .6;
        }
        t += 6;
    }

    
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    model = transform(0.0f, 0.0f, 8.0f-.3f, 0.0f, 0.0f, 0.0f, .3, 8.0f, .3f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    model = transform(11.0f, 0.0f, 8.0f-.3f, 0.0f, 0.0f, 0.0f, .3f, 8.0f, 0.3f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    model = transform(20.0f, 0.0f, 8.0f-.3f, 0.0f, 0.0f, 0.0f, .3f, 8.0f, 0.3f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    model = transform(20.0f, 0.0f, 24.0f-.3f, 0.0f, 0.0f, 0.0f, .3f, 8.0f, 0.3f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);





    //stairs
    //f = 0;    
    //for (int i = 0; i < 10; i++)
    //{
    //    //rail l1
    //    model = transform(27.0f, 0.0f + (f), 1.0f + (f), 0.0f, 0.0f, 0.0f, 5.0f, 0.35f, 1.0f);
    //    cube.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
    //    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //    f += 1;
    //}








    ////fence l2
    //f = 0;
    //for (int i = 0; i < 28; i++)
    //{
    //    //rail l1
    //    model = transform(0.0f+(f), 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.35f, 9.0f, 1.0f);
    //    cube.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
    //    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //    f += 1;
    //}
    //f = 0;
    //for (int i = 0; i < 23; i++)
    //{
    //    //rail l1
    //    model = transform(27.0f , 8.0f, 1.0f + (f), 0.0f, 0.0f, 0.0f, 1.0f, 9.0f, 0.35f);
    //    cube.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
    //    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //    f += 1;
    //}

    


}

//void bus(Cube& cube, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
void bus(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Cube& cube_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    //glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-35.0f, 0.065f, 68.86783f));
    //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.5f, 1.0f));
    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    glm::mat4 model; 

    //almari_tex black_tex ch_wood_tex cushion_tex

    //model = transform(-50.0f, 0.065f, 48.86783f, 0.0f, 0.0f, 0.0f, 39.0f, .5f, 1.0f);
    //cube.setTextureProperty(bus_side_right_tex, bus_side_right_tex, 32.0f);
    //demo->drawCubeWithTexture(lightingShader, alTogether * model);
    ////cube.drawCubeWithTexture(lightingShader, alTogether * model);

    //model = transform(-35.0f, 0.065f, 68.86783f, 0.0f, 0.0f, 0.0f, 39.0f, .5f, 1.0f);
    //cube.setTextureProperty(bus_side_left_tex, bus_side_left_tex, 32.0f);
    //demo->drawCubeWithTexture(lightingShader, alTogether * model);
    ////cube.drawCubeWithTexture(lightingShader, alTogether * model);

    //model = transform(-5.0f, 0.065f, 88.86783f, 0.0f, 0.0f, 0.0f, 19.0f, .5f, 1.0f);
    //cube.setTextureProperty(bus_front_tex, bus_front_tex, 32.0f);
    //demo->drawCubeWithTexture(lightingShader, alTogether * model);
    ////cube.drawCubeWithTexture(lightingShader, alTogether * model);

    //model = transform(0.0f, 0.065f, 108.86783f, 0.0f, 0.0f, 0.0f, 19.0f, .5f, 1.0f);
    //cube.setTextureProperty(bus_back_tex, bus_back_tex, 32.0f);
    //demo->drawCubeWithTexture(lightingShader, alTogether * model);
    ////cube.drawCubeWithTexture(lightingShader, alTogether * model);
   
    float p1 = 1.0f;
    float p2 = 2.5f;
    float p3 = 8.0f;
    float p4 = 2.5f;
    float p5 = 1.0f;
    float panel_y = 5.0f;
    float panel_z = 0.35f;

    
    float floor_x = p1 + p2 + p3 + p4 + p5;
    float floor_y = 0.35f;
    float floor_z = 5.0f;


    float front_x = 0.35f;
    float front_y = floor_y + 5.0f;
    float front_z = 5.0f;



    //bus floor
    model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, p1 + p2 + p3 + p4 + p5, floor_y, floor_z + panel_z);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //bus ceiling
    model = transform(0.0f - front_x, 0.0f + front_y, 0.0f, 0.0f, 0.0f, 0.0f, p1 + p2 + p3 + p4 + p5 + (2 * front_x), panel_z, panel_y + (panel_z));
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //bus front
    model = transform(0.0f - front_x, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, front_x, front_y, front_z + panel_z);
    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //front window
    model = transform(0.0f - front_x - .1, 2.3f, 0.38f, 0.0f, 0.0f, 0.0f, front_x - .05, front_y / 2, front_z - .45);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //front headlight1
    model = transform(0.0f - front_x - .1, 0.5f, 0.38f, 0.0f, 0.0f, 0.0f, front_x - .05, 0.5f, 1.0f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //front headlight2
    model = transform(0.0f - front_x - .1, 0.5f, 4.0f, 0.0f, 0.0f, 0.0f, front_x - .05, 0.5f, 1.0f);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);



    //bus back 
    model = transform(0.0f + floor_x, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, front_x, front_y, front_z + panel_z);
    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //back window
    model = transform(0.0f - front_x + floor_x + .45, 2.3f, 0.38f, 0.0f, 0.0f, 0.0f, front_x - .05, front_y / 2, front_z - .45);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);



    //bus back side(right side)
        //first part
    if (bus_door_fl && bus_door_angle < 90)
    {
        bus_door_angle += .4f;
    }
    if (!bus_door_fl && bus_door_angle > 0.0)
    {
        bus_door_angle -= .4f;
    }
    model = transform(0.0f, 0.0f + floor_y, 0.0f, 0.0f, 0.0f, 0.0f, p1, panel_y, panel_z);
    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //front door
    model = transform(p1, 0.0f + floor_y, 0.0f, 0.0f, 0.0f + bus_door_angle, 0.0f, p2, panel_y, panel_z);
    cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //middle
    model = transform(p1 + p2, 0.0f + floor_y, 0.0f, 0.0f, 0.0f, 0.0f, p3, panel_y, panel_z);
    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //back door
    model = transform(p1 + p2 + p3, 0.0f + floor_y, 0.0f, 0.0f, 0.0f + bus_door_angle, 0.0f, p4, panel_y, panel_z);
    cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //last part
    model = transform(p1 + p2 + p3 + p4, 0.0f + floor_y, 0.0f, 0.0f, 0.0f, 0.0f, p5, panel_y, panel_z);
    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //back window1
    model = transform(p1 + p2 + .5, panel_y / 2.3 + floor_y, 0.0f - .1, 0.0f, 0.0f, 0.0f, front_z - 1.8, front_y / 2, front_x - .05);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);

    //back window2
    model = transform(p1 + p2 + 4.2, panel_y / 2.3 + floor_y, 0.0f - .1, 0.0f, 0.0f, 0.0f, front_z - 1.8, front_y / 2, front_x - .05);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);





    //bus front side(left side)
        //first part
    model = transform(0.0f, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f, 0.0f, p1, panel_y, panel_z);
    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //
        //front door
    model = transform(p1, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f - bus_door_angle, 0.0f, p2, panel_y, panel_z);
    cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //
        //middle
    model = transform(p1 + p2, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f, 0.0f, p3, panel_y, panel_z);
    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //
        //back door
    model = transform(p1 + p2 + p3, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f - bus_door_angle, 0.0f, p4, panel_y, panel_z);
    cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //
        //last part
    model = transform(p1 + p2 + p3 + p4, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f, 0.0f, p5, panel_y, panel_z);
    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);

    //front window1
    model = transform(p1 + p2 + .5, panel_y / 2.3 + floor_y, 0.0f + floor_z + .1, 0.0f, 0.0f, 0.0f, front_z - 1.8, front_y / 2, front_x - .05);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);

    //front window2
    model = transform(p1 + p2 + 4.2, panel_y / 2.3 + floor_y, 0.0f + floor_z + .1, 0.0f, 0.0f, 0.0f, front_z - 1.8, front_y / 2, front_x - .05);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);


    ////test cube
    //model = transform(p1 + p2 + 4.2, panel_y / 2.3 + floor_y, 0.0f + floor_z + .1 + 1, 0.0f, 0.0f, 0.0f, front_z - 1.8, front_y / 2, front_x - .05);
    ////cube.setTextureProperty(black_tex, black_tex, 32.0f);
    ////cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);
    //cube.setMaterialisticProperty(glm::vec3(1.0f, 0.0f, 0.0f));
    //cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model);

    

    float w_x = 3.6f;
    float w_y = -1.18f+1.0f;
    float w_z = 3.1f;
    
    float w_sx = 1.3f;
    float w_sy = 1.0f;
    float w_sz = 1.3f;



    //front wheel1
    model = transform(w_x, w_y, w_z, 90.0f, 0.0f, 0.0f, w_sx, w_sy, w_sz);
    Wheel_hollow(curve_cyl, curve_hollow_cyl, cube_cyl, lightingShader, lightingShaderWithTexture, alTogether* model);

    //front wheel2
    model = transform(w_x + 8.2f, w_y, w_z, 90.0f, 0.0f, 0.0f, w_sx, w_sy, w_sz);
    Wheel_hollow(curve_cyl, curve_hollow_cyl, cube_cyl, lightingShader, lightingShaderWithTexture, alTogether* model);
//
    //back wheel1
    model = transform(w_x, w_y, w_z-floor_z+0.8, 90.0f, 0.0f, 0.0f, w_sx, w_sy, w_sz);
    Wheel_hollow(curve_cyl, curve_hollow_cyl, cube_cyl, lightingShader, lightingShaderWithTexture, alTogether* model);
    //
    //back wheel2
    model = transform(w_x + 8.2f, w_y, w_z-floor_z+0.8, 90.0f, 0.0f, 0.0f, w_sx, w_sy, w_sz);
    Wheel_hollow(curve_cyl, curve_hollow_cyl, cube_cyl, lightingShader, lightingShaderWithTexture, alTogether* model);




    ////test strip
    //model = transform(0.0f, 0.0f, -2.0f, 0.0f, 0.0f, 0.0f, 10.0f, .5f, 1.0f);
    //cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    ////demo->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);


}




//void bus2(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    //glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    //glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-35.0f, 0.065f, 68.86783f));
//    //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
//    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
//    //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
//    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.5f, 1.0f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    glm::mat4 model;
//
//
//
//    //model = transform(-50.0f, 0.065f, 48.86783f, 0.0f, 0.0f, 0.0f, 39.0f, .5f, 1.0f);
//    //cube.setTextureProperty(bus_side_right_tex, bus_side_right_tex, 32.0f);
//    //demo->drawCubeWithTexture(lightingShader, alTogether * model);
//    ////cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //model = transform(-35.0f, 0.065f, 68.86783f, 0.0f, 0.0f, 0.0f, 39.0f, .5f, 1.0f);
//    //cube.setTextureProperty(bus_side_left_tex, bus_side_left_tex, 32.0f);
//    //demo->drawCubeWithTexture(lightingShader, alTogether * model);
//    ////cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //model = transform(-5.0f, 0.065f, 88.86783f, 0.0f, 0.0f, 0.0f, 19.0f, .5f, 1.0f);
//    //cube.setTextureProperty(bus_front_tex, bus_front_tex, 32.0f);
//    //demo->drawCubeWithTexture(lightingShader, alTogether * model);
//    ////cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //model = transform(0.0f, 0.065f, 108.86783f, 0.0f, 0.0f, 0.0f, 19.0f, .5f, 1.0f);
//    //cube.setTextureProperty(bus_back_tex, bus_back_tex, 32.0f);
//    //demo->drawCubeWithTexture(lightingShader, alTogether * model);
//    ////cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//
//
//    //1st middle
//    model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.5f, 5.0f, 0.35f);
//    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //2nd middle
//    model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.5f, 5.0f, 0.35f);
//    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    ////strip
//    //model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, .5f, 1.0f);
//    //cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
//    ////demo->drawCubeWithTexture(lightingShader, alTogether * model);
//    //cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//}


















































void Floor(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -1.0, 0));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(60.0f, 1.0f, 60.0f));
    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    model = translateMatrix * scaleMatrix;
    //model = scaleMatrix * translateMatrix;

    //moveMatrix = rotateZMatrix * moveMatrix;

    cube.drawCubeWithTexture(lightingShader, alTogether * model);
}


//void FWall(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(35.0f, 12.0f, 0.1f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model = scaleMatrix * translateMatrix;
//    //moveMatrix = rotateZMatrix * moveMatrix;
//    lightingShader.setMat4("model", alTogether * model);
//    //drawCube(cubeVAO, lightingShader, alTogether * model, 0.53f, 0.81f, 0.92f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//void ladder(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 model = transform(-2.0, -.8, -4, 0, 0, 0, .15, 5.3, .15);
//    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    model = transform(-1.4, -.8, -4, 0, 0, 0, .15, 5.3, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, -.4, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, 0, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, 0.4, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, 0.8, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    model = transform(-2.0, 1.2, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, 1.6, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//void bed(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    float baseHeight = 0.3;
//    float width = 1;
//    float length = 2;
//    float pillowWidth = 0.3;
//    float pillowLength = 0.15;
//    float blanketWidth = 1.0;
//    float blanketLength = 1.8;
//    float headHeight = 0.6;
//
//    //base
//    glm::mat4 model = glm::mat4(1.0f);
//    glm::mat4 translate = glm::mat4(1.0f);
//    glm::mat4 translate2 = glm::mat4(1.0f);
//    glm::mat4 scale = glm::mat4(1.0f);
//    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
//    cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32.0);
//    cube.drawCubeWithTexture(lightingShader, model);
//
//    //foam
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, baseHeight / 2, 0));
//    scale = glm::scale(model, glm::vec3(width, 0.06, length));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate2 * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);
//    cube.setTextureProperty(cushion_tex, cushion_tex, 32.0);
//    cube.drawCubeWithTexture(lightingShader, model);
//
//    //pillow 1
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3((width / 4) - (0.1 + pillowWidth / 2), baseHeight / 2 + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
//    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
//    translate = glm::translate(model, glm::vec3(-0.46, 0, -0.5));
//    model = alTogether * translate2 * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0);
//    cube.drawCubeWithTexture(lightingShader, model);
//
//    //pillow 2
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3((-width / 4) + (0.1 + pillowWidth / 2), baseHeight / 2 + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
//    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
//    translate = glm::translate(model, glm::vec3(-0.21, 0.0, -0.5));
//    model = alTogether * translate2 * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0);
//    cube.drawCubeWithTexture(lightingShader, model);
//
//    //blanket
//    /*model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, baseHeight/2 + 1 * 0.06, -(length / 2 - 0.025) + blanketLength / 2));
//    scale = glm::scale(model, glm::vec3(blanketWidth, 0.015, blanketLength));
//    translate = glm::translate(model, glm::vec3(-0.5, -0.05, -0.41));
//    model = alTogether * translate2 * translate * scale ;
//    drawCube(cubeVAO, lightingShader, model, 0.541, 0.169, 0.886);*/
//
//    //head
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, 0, (length / 2 - 0.02 / 2) + 0.02));
//    scale = glm::scale(model, glm::vec3(width, headHeight, 0.02));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate2 * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
//    cube.setTextureProperty(almari_tex, almari_tex, 32.0);
//    cube.drawCubeWithTexture(lightingShader, model);
//}
//
//void Table(Cube &cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    //cube.setMaterialisticProperty(glm::vec3(0.862f, 0.46f, 0.321f));
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.2f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.2f, 3.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.6f, 0.0f, -0.1f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix1 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix1 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model1 = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model1);
//
//    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
//    translateMatrix2 = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, -0.1f));
//    rotateXMatrix2 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix2 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    model2 = rotateXMatrix2 * translateMatrix2 * scaleMatrix2;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model2);
//
//    glm::mat4 translateMatrix3, rotateXMatrix3, rotateYMatrix3, rotateZMatrix3, scaleMatrix3, model3;
//    translateMatrix3 = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, -1.49f));
//    rotateXMatrix3 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix3 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix3 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix3 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model3 = rotateXMatrix3 * translateMatrix3 * scaleMatrix3;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model3);
//    // Modelling Transformation
//    glm::mat4 translateMatrix4, rotateXMatrix4, rotateYMatrix4, rotateZMatrix4, scaleMatrix4, model4;
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(-0.6f, 0.0f, -1.49f));
//    rotateXMatrix4 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix4 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix4 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    model4 = rotateXMatrix4 * translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//}
//
//void Sofa(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    //Back
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.19f, 0.0f, 0.0f));
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.55f, 2.0f, 0.5f));
//    alTogether = rotateYMatrix * alTogether;
//    model = scaleMatrix * translateMatrix;
//    //cube.setMaterialisticProperty(glm::vec3(0.4f, 0.226f, 0.44f));
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //Seat
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.5f, 0.0f));//translate_X, translate_Y, translate_Z
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 1.0f, 2.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//    ////left hand
//    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(1.1f, -0.5f, -1.0f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 2.0f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.setMaterialisticProperty(glm::vec3(0.52f, 0.39f, 0.31f));
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    ////right hand
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.79f, -0.5f, -1.0f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 2.0f));
//    model = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    ////left Leg
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.79f, 0.35f, -1.0f));//translate_X, translate_Y, translate_Z
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 0.8f, 2.0f));
//    model = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    ////right Leg
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(1.08f, 0.35f, -1.0f));//translate_X, translate_Y, translate_Z
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 0.8f, 2.0f));
//    model = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//}
//
//void Ladder(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.77006, -0.75, 14.1102));
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//
//    ladder(cube, lightingShader, translateMatrix * rotateZMatrix);
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.77006, 0.62, 14.75));
//    ladder(cube, lightingShader, translateMatrix * rotateZMatrix);
//}
//
//void DiningTable(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 0, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 0.3f, 3.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//    //drawCube(cubeVAO, lightingShader, alTogether * model, 0.478, 0.573, 0.62);
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 2.52, -0.025));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.5f, 0.05f, 3.5f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //ourShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
//    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.6f, 0.0f, -0.1f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix1 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix1 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model1 = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model1);
//
//    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
//    translateMatrix2 = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, -0.1f));
//    rotateXMatrix2 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix2 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    model2 = rotateXMatrix2 * translateMatrix2 * scaleMatrix2;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model2);
//
//    glm::mat4 translateMatrix3, rotateXMatrix3, rotateYMatrix3, rotateZMatrix3, scaleMatrix3, model3;
//    translateMatrix3 = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, -1.49f));
//    rotateXMatrix3 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix3 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix3 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix3 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model3 = rotateXMatrix3 * translateMatrix3 * scaleMatrix3;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model3);
//    // Modelling Transformation
//    glm::mat4 translateMatrix4, rotateXMatrix4, rotateYMatrix4, rotateZMatrix4, scaleMatrix4, model4;
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(-0.6f, 0.0f, -1.49f));
//    rotateXMatrix4 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix4 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix4 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    model4 = rotateXMatrix4 * translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//}
//
//void Tool(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.2f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2f, 2.0f));
//    model = scaleMatrix * translateMatrix;
//    //drawCube(cubeVAO, lightingShader, alTogether * model, 0.0f, 0.4f, 0.18f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //Leg
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;//translate_X, translate_Y, translate_Z
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.05f, -0.86f, 0.36f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.5f, 1.7f, 0.5f));
//    model1 = translateMatrix1 * scaleMatrix1;
//    cube.setMaterialisticProperty(glm::vec3(0.52f, 0.39f, 0.31f));
//    cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model1);
//
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.16f, -4.27f, 0.21f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 1.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model);
//}
//
//void Chair(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    //Seat
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.22, -1.0, 0.05));//
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.1f, 1.0f));
//    alTogether = rotateYMatrix * alTogether;
//    model = scaleMatrix * translateMatrix;
//    //drawCube(cubeVAO, lightingShader, alTogether * model, 0.165, 0.435, 0.451);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //Back
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.22, 0.15f, 0.5f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.7f, 0.1f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.1, -0.1f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
//    model1 = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model1);
//
//    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
//    translateMatrix2 = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.1, -0.5f));
//    rotateXMatrix2 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix2 = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
//    model2 = rotateXMatrix2 * translateMatrix2 * scaleMatrix2;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model2);
//
//    // Modelling Transformation
//    glm::mat4 identityMatrix3 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix3, rotateXMatrix3, rotateYMatrix3, rotateZMatrix3, scaleMatrix3, model3;
//    translateMatrix3 = glm::translate(identityMatrix, glm::vec3(0.2f, 0.1, -0.1f));
//    rotateXMatrix3 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix3 = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
//    model3 = rotateXMatrix3 * translateMatrix3 * scaleMatrix3;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model3);
//    // Modelling Transformation
//
//    glm::mat4 translateMatrix4, rotateXMatrix4, rotateYMatrix4, rotateZMatrix4, scaleMatrix4, model4;
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(0.2f, 0.1, -0.5f));
//    rotateXMatrix4 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
//    model4 = rotateXMatrix4 * translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.1f, 0.05f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
//    model4 = translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(0.2f, -0.1f, 0.05f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
//    model4 = translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//
//}
//
//void Bookself(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
//{
//    // base
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 0.0f, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.1f, 0.5f));
//    model = translateMatrix * scaleMatrix;
//    //drawCube(cubeVAO, lightingShader, alTogether * model,);
//    //cube.setMaterialisticProperty(glm::vec3(0.462f, 0.258f, 0.145f));
//    cube.setTextureProperty(black_tex, black_tex, 32.0f);
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 1.3, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.1f, 0.5f));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    // mid 1
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.55, 1.0f, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.1f, 0.5f));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    // mid 2
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.55, .7, 1));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .1, 0.5));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    // mid 3
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.55, .35, 1));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .1, 0.5));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    // left
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, .05, 1));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 2.5, 0.5));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    //right
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8, .05, 1));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 2.5, 0.5));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    //Books
//    float colors[] = { 0.545455, 0.909091, 0.545455, 0.181818, 0.0909091, 0.363636, 0.0, 0.545455, 0.272727, 0.0909091, 0.727273, 0.636364, 0.454545, 0.272727, 0.636364, 0.363636, 0.818182, 0.909091, 0.181818, 0.0, 0.909091, 0.727273, 0.454545, 0.0, 0.363636, 0.545455, 0.0, 0.909091, 0.272727, 0.909091, 0.909091, 0.636364, 0.909091, 0.272727, 0.636364, 0.818182 };
//    int k = 0;
//    for (float j = 0; j <= 0.9; j = j + 0.3)
//    {
//        for (float i = 0; i < 0.2; i = i + 0.06)
//        {
//            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.75 - i, 1.05 - j, 1.0));
//            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05, 0.5, 0.6));
//            model = translateMatrix * scaleMatrix;
//            k = k + 3;
//            //drawCube(cubeVAO, lightingShader, alTogether * model, colors[k], colors[k + 1], colors[k + 2]);
//            cube.setMaterialisticProperty(glm::vec3(colors[k], colors[k + 1], colors[k + 2]));
//            cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model);
//        }
//    }
//
//
//}
//
//void TV(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 4.0f, 0.2f));
//    model = scaleMatrix * translateMatrix;
//    cube.setMaterialisticProperty(glm::vec3(0.0f, 0.0f, 0.0f));
//    cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model);
//
//}
//
//void Show(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.70f, 3.70f, 0.2f));
//    model = scaleMatrix * translateMatrix;
//    //cube.setMaterialisticProperty(glm::vec3(0.0f, 0.0f, 0.0f));
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//void Door(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 9.0f, 0.2f));
//    model = scaleMatrix * translateMatrix;
//    //cube.setMaterialisticProperty(glm::vec3(0.0f, 0.0f, 0.0f));
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//void TV_Trolly(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.0f, 2.0f, 1.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//void Wardrobe(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    //WARDROBE 
//        //back
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    model = transform(6.95, -0.75, -8.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 7.0f, 4.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //left
//    model = transform(5.95, -0.75, -8.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 7.0f, 0.1f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    //right
//    model = transform(5.95, -0.75, -6.55, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 7.0f, 0.1f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //upper
//    model = transform(5.95, 2.75, -8.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.1f, .1f, 4.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //.5 down
//    model = transform(5.95, -0.5 - 0.025, -8.5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //WARDROBE front self
//    model = transform(5.95, -0.5 + 0.025, -8.5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 6.45f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //WARDROBE front self border
//    model = transform(5.95 - 0.01, -0.5 + 0.025, -7.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 6.45f, 0.02f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//void Shokez(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    model = transform(6.95, -0.75, .5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 7.0f, 4.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //left
//    model = transform(5.95, -0.75, .5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 7.0f, 0.1f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //right
//    model = transform(5.95, -0.75, 2.45, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 7.0f, 0.1f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//    //upper
//    model = transform(5.95, 2.75, .5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.1f, .1f, 4.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    //3
//    model = transform(5.95, 0.75 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //.5
//    model = transform(5.95, -0.5 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //front self
//    model = transform(5.95, -0.5 + 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 2.4f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //front self border
//    model = transform(5.95 - 0.01, -0.5 + 0.025, 1.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 2.4f, 0.02f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //4
//    model = transform(5.95, 1.25 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //5
//    model = transform(5.95, 1.75 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    //6 
//    model = transform(5.95, 2.25 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1.8f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//    //bokself end
//
//
//}

