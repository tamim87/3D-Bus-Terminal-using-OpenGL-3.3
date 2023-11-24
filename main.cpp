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

void Floor(Cube& cube, Shader& lightingShader, glm::mat4 alTogether);
void bus(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Cube& cube_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void Wheel(Curve& curve_cyl, Curve& curve_circle, Shader& lightingShader, glm::mat4 alTogether);
void Wheel_hollow(Curve& curve_cyl, Curve& curve_hollow_cyl, Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void terminal(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void lobby(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void seat(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void windmill_blades(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void windmill_body(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void road(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void ticket_counter(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void fence(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void make_boat(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Curve& tree, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void make_tree(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Curve& tree, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void make_tree2(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Curve& tree, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
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
unsigned int floor_tiles_tex;
unsigned int ch_wood_tex;
unsigned int almari_tex;
unsigned int cushion_tex;
unsigned int black_tex;

unsigned int road_tex;
unsigned int stone_tex;
unsigned int sq_tile_tex;
unsigned int sun_tex;
unsigned int leaf_tex;
unsigned int marbel_tex;
unsigned int door_tex;
unsigned int tyre_tex;
unsigned int window_tex;
unsigned int decal_body_tex;
unsigned int counter_tex;
unsigned int wood_tex;





Camera camera(glm::vec3(-10.0f, 13.1f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);

// lights
// positions of the point lights
glm::vec3 LightPositions[] = {
    glm::vec3(30.0, 20.0, 35.0), //directional light1 - white
    glm::vec3(-24, 7, 8), //spot light - red
    glm::vec3(0,-4.92 ,0), //point light1 - green -> there are some problems in this light, better not use it
    glm::vec3(- 28 ,5.1 ,13. - 20), //point light2 - blue
    glm::vec3(20, 6 ,-9), //point light3 - yellow
    glm::vec3(-35.0, 20.0, -35.0), //directional light2 - grey
    glm::vec3(-35.0, 20.0, -35.0) //directional light3 - sphere for sun
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
SpotLight spotlight1(

    LightPositions[1].x, LightPositions[1].y, LightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
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
    .8f, .8f, .8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 0.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
DirLight dirlight3(

    LightPositions[6].x, LightPositions[6].y, LightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.5f, 1.5f, 1.5f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 0.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);


// light settings
bool pointLightOn1 = true;
bool pointLightOn2 = true;
bool pointLightOn3 = true;
bool dirLightOn1 = true;
bool dirLightOn2 = true;
bool dirLightOn3 = false;
bool spotLightOn1 = true;
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


vector<float>wheel_lid_vertices = {
    0.1750, 1.9850, 5.1000,
    -0.3050, 1.9650, 5.1000,
    -0.5800, 1.9650, 5.1000,
};

//cyllinder shape using bezier
vector<float>wheel_vertices = {
    -0.7300, 2.3200, 5.1000,
    -0.7400, 2.1250, 5.1000,
    -0.7350, 1.9250, 5.1000,
    -0.7250, 1.6700, 5.1000,
    -0.7250, 1.4700, 5.1000,
    -0.7250, 1.2600, 5.1000,
    -0.7200, 1.0350, 5.1000
};

//hollow cyllinder shape using bezier
vector<float>hollow_cyllinder_vertices = 
{
    -0.4900, 1.9100, 5.1000,
    -1.5750, 1.7800, 5.1000,
};

//alternative
vector<float>tree_vertices = {
    -0.3800, 1.8200, 5.1000,
    -0.4600, -0.2750, 5.1000,
    -0.9950, 1.4450, 5.1000,
    -1.0400, 0.1600, 5.1000,
    -1.5800, 0.8500, 5.1000,
};

//alternative
vector<float>share_tree_vertices = {
    -1.5100, 1.0500, 5.1000,
    -1.0150, 1.5850, 5.1000,
    -0.9450, 0.4250, 5.1000,
    -0.2300, 2.2150, 5.1000,
    -0.1250, -0.2200, 5.1000,
};

//main vertices for tree using bezier
vector<float>tree2_vertices = {
    0.0250, 1.6600, 5.1000,
    -0.7250, 1.3900, 5.1000,
    -0.2450, 1.3550, 5.1000,
    -0.8500, 1.2300, 5.1000,
    -0.2850, 1.2100, 5.1000,
    -0.8650, 0.7700, 5.1000,
    -0.3250, 0.8300, 5.1000
};

vector<float>apple_shape_vertices = {
    -0.7250, 2.0650, 5.1000,
    -1.3300, 1.3600, 5.1000,
    -0.7450, 0.5650, 5.1000,
};

vector<float>baloon_shape_vertices = {
    -0.0550, 2.3550, 5.1000,
    -1.1900, 1.3050, 5.1000,
    -0.0500, 0.1700, 5.1000,
};

vector<float>mushroom_shape_vertices = {
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
Cube* stone_tile_cube_pointer;
Cube* sq_tile_cube_pointer;
Cube* rectangle_cube_pointer;



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
    string marbel = "marbel.jpg";
    string cushion = "cushion.jpg";
    string ch_wood = "ch_wood.jpg";
    string wall = "wall.jpg";
    string walls = "walls.jpg";
    string black = "black.jpg";
    string almari = "almari.jpg";
    string door = "busdoor.png";
    string tyre = "tyre1.jpg";
    string window_ = "window4.png";
    string decal_body = "decal3.jpg";
    string counter = "counter.jpg";



    string         road_o = "road.jpg";
    string         stone  = "stone_ground.jpg";
    string        sq_tile = "sq_tile.jpg";
    string        sunP    = "sun.png";
    string        leaf    = "leaf.jpg";



    //unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    marbel_tex = loadTexture(marbel.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    floor_tiles_tex = loadTexture(f_tiles.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    ch_wood_tex = loadTexture(ch_wood.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    almari_tex = loadTexture(almari.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    cushion_tex = loadTexture(cushion.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    black_tex = loadTexture(black.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    road_tex = loadTexture(road_o.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    stone_tex = loadTexture(stone.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    sq_tile_tex = loadTexture(sq_tile.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    sun_tex = loadTexture(sunP.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    leaf_tex = loadTexture(leaf.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    tyre_tex = loadTexture(tyre.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    door_tex = loadTexture(door.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    window_tex = loadTexture(window_.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    decal_body_tex = loadTexture(decal_body.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    counter_tex = loadTexture(counter.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    wood_tex = loadTexture(wood.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    


    Cube cube = Cube(black_tex, black_tex, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube tiles_cube = Cube(floor_tiles_tex, floor_tiles_tex, 92.0f, 0.0f, 0.0f, 20.0f, 20.0f);
    Cube cube_cyl = Cube(cushion_tex, cushion_tex, 32.0, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube stone_cube = Cube(stone_tex, stone_tex, 92.0f, 0.0f, 0.0f, 12.0f, 12.0f);
    Cube sq_tile_cube = Cube(sq_tile_tex, sq_tile_tex, 92.0f, 0.0f, 0.0f, 4.0f, 5.0f);
    Cube rectangle_cube = Cube(counter_tex, counter_tex, 92.0f, 0.0f, 0.0f, 3.0f, 10.0f);

    
    sq_tile_cube_pointer = &sq_tile_cube;
    stone_tile_cube_pointer = &stone_cube;
    rectangle_cube_pointer = &rectangle_cube;


    //Sphere2 sphere = Sphere2() ;
    //sphere.setDefaults();
    Sphere2  sphere= Sphere2(3.0f, 144, 72, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, ch_wood_tex,ch_wood_tex, 0.0f, 0.0f, 20.0f, 20.0f);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    Curve wheel(wheel_vertices, black_tex,black_tex, 1.0f);
    Curve wheel_hollow(hollow_cyllinder_vertices, black_tex, black_tex, 1.0f);
    Curve boat(tree_vertices, ch_wood_tex, ch_wood_tex, 1.0f);
    Curve tree(mushroom_shape_vertices, sq_tile_tex, sq_tile_tex, 1.0f);
    Curve tree2(tree2_vertices, black_tex, black_tex, 1.0f);

    if (dirLightOn1 && dirLightOn2)
    {
        dirlight3.turnOff();
        glClearColor(0.435f, 0.675f, 0.949f, 1.0f);
        dirLightOn3 = !dirLightOn3;
    }

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
        //glClearColor(0.435f, 0.675f, 0.949f, 1.0f);
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
        spotlight1.setUpPointLight(lightingShaderWithTexture);
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        dirlight2.setUpPointLight(lightingShaderWithTexture);
        dirlight3.setUpPointLight(lightingShaderWithTexture);
        //dirligh1.turnOff();
        //spotlight1.turnOff();
        pointlight1.turnOff();
        //pointlight2.turnOff();
        //pointlight3.turnOff();
        //dirligh2.turnOff();
        //dirligh3.turnOff();






        

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
        tiles_cube.setTextureProperty(sq_tile_tex, sq_tile_tex, 52.0f);
        Floor(tiles_cube, lightingShaderWithTexture, translateMatrix);


        //model = transform(0.0f, 2.69f, -15.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        //bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);






        //light_for_materialistic_property
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        dirlight1.setUpPointLight(lightingShader);
        spotlight1.setUpPointLight(lightingShader);
        pointlight1.setUpPointLight(lightingShader);
        pointlight2.setUpPointLight(lightingShader);
        pointlight3.setUpPointLight(lightingShader);
        dirlight2.setUpPointLight(lightingShader);
        dirlight3.setUpPointLight(lightingShader);
        //dirlight1.turnOff();
        //spotlight1.turnOff();
        pointlight1.turnOff();
        //pointlight2.turnOff();
        //pointlight3.turnOff();
        //dirligh2.turnOff();
        //dirligh3.turnOff();


        


        float sx = 0.5f;
      
        
        if (!bus_move_fl1 && bus_move_tx1 < 13)
        {
            bus_move_tx1 += .08;
        }
        if (bus_move_fl1 && bus_move_tx1 > -28)
        {
            bus_move_tx1 -= .08;
        }
        model = transform(0.0f + bus_move_tx1, .79, 13.0+6, 0.0f, 0.0f, 0.0f, .7, .7, .7);
        bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);

        if (bus_move_fl2 && bus_move_tx2 < 13)
        {
            bus_move_tx2 += .08;
        }
        if (!bus_move_fl2 && bus_move_tx2 > -28)
        {
            bus_move_tx2 -= .08;
        }
        model = transform(14.0f + bus_move_tx2, .79, 25 +2.5, 0.0f, 180.0f, 0.0f, .7, .7, .7);
        bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);

        model = transform(-30.0f, 0.0f, 18.0f, 0.0f, 0.0f, 0.0f, 60.0f, 1, .68);
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        road(cube, lightingShader, lightingShaderWithTexture, model);


        //static bus 1
        model = transform(0.0f, .79, 0.0f, 0.0f, 90.0f, 0.0f, .7, .7, .7);
        //bus(cube, lightingShaderWithTexture, model);
        bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);
        //static bus 2
        model = transform(11.0f, .79, -5.0f, 0.0f, -90.0f, 0.0f, .7, .7, .7);
        //bus(cube, lightingShaderWithTexture, model);
        bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);


        model = transform(-29, 0.0f, 0, 0.0f, 90, 0.0f, .7,.7,.7);
        terminal(cube, lightingShader, lightingShaderWithTexture, model);


        model = transform(12.5, 0, -20.5, 0.0f, 0.0f, 0.0f, .8, .8, .8);
        //lightingShaderWithTexture.setMat4("view", view* transform(+28.0f, 0.0f, +22.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
        //lightingShader.setMat4("view", view* transform(+28.0f, 0.0f, +22.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
        lobby(cube, lightingShader, lightingShaderWithTexture, model);


//testing pos
//model = transform(0.0f, 1.15f, 0.0f, 0.0f, 90.0f, 0.0f, 1.0f, 1.0f, 1.0f);
//bus(cube, wheel, wheel_hollow, cube_cyl, lightingShader, lightingShaderWithTexture, model);


        model = transform(2, 0, -20, 0.0f, 0.0f, 0.0f, 1, 1.0f, 1.0f);
        windmill_body(cube,wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);

        model = transform(0, 0, 0, 0.0f, 0.0f, 0.0f, 1, 1.0f, 1.0f);
        sun_rotate(sphere, lightingShader, lightingShaderWithTexture, model);


        float px = 0;
        float ry = 0;
        for (int i = 0; i < 7; i++)
        {
            model = transform(-24.0f + px, 1, -26, 0.0f, 0.0f + ry, 0.0f, 1.2, 1.2, 1.2);
            make_tree2(cube, wheel, wheel_hollow, tree2, lightingShader, lightingShaderWithTexture, model);
            px += 8;
            ry += 60;
        }
         
    
        //fence for border
        model = transform(-30.0f, 0.0f, 30.0f, 0.0f, 0.0f, 0.0f, 1, 1, 1);
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        fence(cube, lightingShader, lightingShaderWithTexture, model);
        //fence for border
        model = transform(-30.0f, 0.0f, 30.0f, 0.0f, 90.0f, 0.0f, 1, 1, 1);
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        fence(cube, lightingShader, lightingShaderWithTexture, model);
        //fence for border
        model = transform(-30.0f, 0.0f, -30.0f, 0.0f, 0.0f, 0.0f, 1, 1, 1);
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        fence(cube, lightingShader, lightingShaderWithTexture, model);
        //fence for border
        model = transform(30.0f, 0.0f, -30.0f, 0.0f, -90.0f, 0.0f, 1, 1, 1);
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        fence(cube, lightingShader, lightingShaderWithTexture, model);

        //ticket counter
        model = transform(-30, 0, 23, 0, 90, 0.0f, .75, .75, .75);
        ticket_counter(cube, lightingShader, lightingShaderWithTexture, model);


        //`







        //draw the lamp object(s)
        
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        glm::vec3 LightColor[] = {
        glm::vec3(0.8f, 0.8f, 0.8f),// White - Dirlight1
        glm::vec3(0.0f,  0.0f,  0.0f),//Red - Spot Light 1
        glm::vec3(0.0f,  1.0f,  0.0f),//Green - Point Light 1
        glm::vec3(0.0f,  0.0f,  1.0f),//Blue - Point Light 2
        glm::vec3(1.0f,  1.0f,  0.0f),//Yellow - Point Light 3
        glm::vec3(0.8f, 0.8f, 0.8f),// Grey - Dirlight2
        glm::vec3(0.612f, 0.612f, 0.612f)// Grey - Dirlight3 for sun
        };
        
        for (unsigned int i = 0; i < 7 ; i++)
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

    //if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    //{
    //    if (rotateAxis_X) rotateAngle_X -= 0.1;
    //    else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
    //    else rotateAngle_Z -= 0.1;
    //}
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
        if (dirLightOn1 && dirLightOn2)
        {
            dirlight1.turnOff();
            dirlight2.turnOff();
            dirlight3.turnOn();
            glClearColor(0.0f,0.0f,0.0f,1.0f);
            dirLightOn1 = !dirLightOn1;
            dirLightOn2 = !dirLightOn2;
            dirLightOn3 = !dirLightOn3;
        }
        else
        {
            dirlight1.turnOn();
            dirlight2.turnOn();
            dirlight3.turnOff();
            glClearColor(0.435f, 0.675f, 0.949f, 1.0f);
            dirLightOn1 = !dirLightOn1;
            dirLightOn2 = !dirLightOn2;
            dirLightOn3 = !dirLightOn3;
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
        if (spotLightOn1)
        {
            spotlight1.turnOff();
            spotLightOn1 = !spotLightOn1;
        }
        else
        {
            spotlight1.turnOn();
            spotLightOn1 = !spotLightOn1;
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
            if (spotLightOn1)
                spotlight1.turnAmbientOff();
            if (dirLightOn1)
                dirlight1.turnAmbientOff();
            if (dirLightOn2)
                dirlight2.turnAmbientOff();
            if (dirLightOn3)
                dirlight3.turnAmbientOff();
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
            if (spotLightOn1)
                spotlight1.turnAmbientOn();
            if (dirLightOn1)
                dirlight1.turnAmbientOn();
            if (dirLightOn2)
                dirlight2.turnAmbientOn();
            if (dirLightOn3)
                dirlight3.turnAmbientOn();
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
            if (dirLightOn3)
                dirlight3.turnDiffuseOff();
            if (spotLightOn1)
                spotlight1.turnDiffuseOff();
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
            if (dirLightOn3)
                dirlight3.turnDiffuseOn();
            if (spotLightOn1)
                spotlight1.turnDiffuseOn();
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
            if (dirLightOn3)
                dirlight3.turnSpecularOff();
            if (spotLightOn1)
                spotlight1.turnSpecularOff();
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
            if (dirLightOn3)
                dirlight3.turnSpecularOn();
            if (spotLightOn1)
                spotlight1.turnSpecularOn();
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
    //if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    //{
    //    if (dirLightOn3)
    //    {
    //        dirlight3.turnOff();
    //        dirlight1.turnOn();
    //        dirlight2.turnOn();
    //        glClearColor(0.435f, 0.675f, 0.949f, 1.0f);
    //        dirLightOn3 = !dirLightOn3;
    //        dirLightOn1 = !dirLightOn1;
    //        dirLightOn2 = !dirLightOn2;
    //    }
    //    else
    //    {
    //        dirlight3.turnOn();
    //        dirlight1.turnOff();
    //        dirlight2.turnOff();
    //        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //        dirLightOn3 = !dirLightOn3;
    //        dirLightOn1 = !dirLightOn1;
    //        dirLightOn2 = !dirLightOn2;
    //    }
    //}

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

//cyllinder shape using bezier
void Wheel(Curve& curve_cyl, Curve& curve_circle, Shader& lightingShader, glm::mat4 alTogether)
{
    glm::mat4 model;
    
    //cyllinder
    model = transform(0, 0, 0, 0, 0, 0, 1, .5, 1);
    curve_cyl.draw(lightingShader, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //front circle
    model = transform(0, -2.773, 0, 0, 0, 0, 1.25, 2, 1.25);
    curve_circle.draw(lightingShader, alTogether * model, glm::vec3(0.32f, 0.32f, 0.32f));

    //back circle
    model = transform(0, -3.61, 0, 0, 0, 0, 1.25, 2.1, 1.25);
    curve_circle.draw(lightingShader, alTogether * model, glm::vec3(0.32f, 0.32f, 0.32f));
}

//hollow cyllinder using bezier
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

//for testing(not that good)
void make_tree(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Curve& tree, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    //tree
    model = transform(0, 0, 0, 180, 0, 0, 1, 25, 1);
    tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

}

//main tree function
void make_tree2(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Curve& tree, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    float sx = 3 ,sy = 4, sz = 3;
    float pos = 1.6;
    tree.setTextureProperty(leaf_tex,leaf_tex, 1.0f);


    //tree1 lower part
    model = transform(pos, 0, pos, 0, 0, 0, sx, sy, sz);
    //tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //tree2 lower part
    model = transform(-pos, 0, pos, 0, 0, 0, sx, sy, sz);
    //tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //tree3 lower part
    model = transform(0, 0, -pos, 0, 0, 0, sx, sy, sz);
    //tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));
//
    sx *= .7;
    sz *= .7;
    //tree1 mid part
    model = transform(pos, 1, pos, 0, 0, 0, sx, sy, sz);
    //tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //tree2 mid part
    model = transform(-pos, 1, pos, 0, 0, 0, sx, sy, sz);
    //tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //tree3 mid part
    model = transform(0, 1, -pos, 0, 0, 0, sx, sy, sz);
    //tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //
    sx *= .5;
    sz *= .5;
    //tree1 top part
    model = transform(pos, 1.3, pos, 0, 0, 0, sx, sy, sz);
    //tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //tree2 top part
    model = transform(-pos, 1.3, pos, 0, 0, 0, sx, sy, sz);
    //tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //tree3 top part
    model = transform(0, 1.3, -pos, 0, 0, 0, sx, sy, sz);
    //tree.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    tree.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));


    //tree body x3 (for three trees)
    pos = 1.6;
    float cyl_sx = 1;
    curve_cyl.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    model = transform(pos, -5, pos, 0, 0, 0, cyl_sx,4, cyl_sx);
    curve_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    curve_cyl.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    model = transform(-pos, -5, pos, 0, 0, 0, cyl_sx, 4, cyl_sx);
    curve_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    curve_cyl.setTextureProperty(ch_wood_tex, ch_wood_tex, 1.0f);
    model = transform(0, -5, -pos, 0, 0, 0, cyl_sx, 4, cyl_sx);
    curve_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));
}

//incomplete, didn't use 
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
    sun_rotate_axis += .3;

    //around y axis rotate
    model = transform(px, 20, pz, 0.0f, 0.0f + sun_rotate_y, 0.0f, 1, 1.0f, 1.0f) * model;
    sphere.setRadius(5);
    sphere.setTextureProperty(sun_tex, sun_tex, 1.0f);
    sphere.drawSphereWithTexture(lightingShaderWithTexture, model);
    sun_rotate_y += .28;

    dirlight3.upd_dirlight_pos(lightingShaderWithTexture, px+8, 20+10, pz+8, 1);
}

void ticket_counter(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;
    float baseHeight = 0.01;
    float width = 30.0;
    float length = 20.5;
    float tx = 0.0;
    float ty = 0.0;
    float tz = 0.0;

    //back1
    model = transform(12, baseHeight, 4, 0.0f, 0.0f, 0.0f, 4, 10, baseHeight + .8);
    //cube.setTextureProperty(counter_tex, counter_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    rectangle_cube_pointer->setTextureProperty(counter_tex, counter_tex, 32.0f);
    rectangle_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);


    //back2
    model = transform(12 + 4, baseHeight, 4, 0.0f, 0.0f, 0.0f, 4, 10, baseHeight + .8);
    //cube.setTextureProperty(marbel_tex, marbel_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    rectangle_cube_pointer->setTextureProperty(counter_tex, counter_tex, 32.0f);
    rectangle_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //back3
    model = transform(12 + 4 + 4, baseHeight, 4, 0.0f, 0.0f, 0.0f, 4, 10, baseHeight + .8);
    //cube.setTextureProperty(marbel_tex, marbel_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    rectangle_cube_pointer->setTextureProperty(counter_tex, counter_tex, 32.0f);
    rectangle_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //back4
    model = transform(12 + 4 + 4 + 4, baseHeight, 4, 0.0f, 0.0f, 0.0f, 4, 10, baseHeight + .8);
    //cube.setTextureProperty(marbel_tex, marbel_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    rectangle_cube_pointer->setTextureProperty(counter_tex, counter_tex, 32.0f);
    rectangle_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //top cover
    model = transform(12 - 1, baseHeight + 10, 4 - 1, 0.0f, 0.0f, 0.0f, 16 + 2, baseHeight + .6, 8);
    cube.setTextureProperty(marbel_tex, marbel_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    tx = 0;
    ty = -.9;
    tz = 0;
    //ticket counter surface
    model = transform(12 + 1 + tx, ty + baseHeight + 10 - 6, tz + 4 - 1 + 1.8, 0.0f, 0.0f, 0.0f, 16 + 2 - 4, baseHeight + .2 + .2, 8 - 5);
    cube.setTextureProperty(marbel_tex, marbel_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    //cube.setMaterialisticProperty(glm::vec3(0.616, 0.667, 0.69));
    //cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model);



    //counter standing area separators
    tx = .2, ty = 3.5, tz = 4.5;
    float px = 13, pz = 8;
    float f = 0;
    for (int i = 0; i < 5; i++)
    {
        model = transform(0 + px + f, 0, 0 + pz, 0.0f, 0.0f, 0.0f, tx, ty, tx);
        cube.setTextureProperty(road_tex, road_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

        model = transform(0 + px + f, ty, 0 + pz, 0.0f, 0.0f, 0.0f, tx, tx, tz);
        cube.setTextureProperty(road_tex, road_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

        model = transform(0 + px + f, 0, tz + pz - tx, 0.0f, 0.0f, 0.0f, tx, ty, tx);
        cube.setTextureProperty(road_tex, road_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        f += 3.5;
    }
    
    f = 0;
    for (int i = 0; i < 5; i++)
    {
        model = transform(0 + px + f, 0, 0 + pz + tz + tx + 1, 0.0f, 0.0f, 0.0f, tx, ty, tx);
        cube.setTextureProperty(road_tex, road_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

        model = transform(0 + px + f, ty, 0 + pz + tz + tx + 1, 0.0f, 0.0f, 0.0f, tx, tx, tz);
        cube.setTextureProperty(road_tex, road_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

        model = transform(0 + px + f, 0, tz + pz - tx + tz + tx + 1, 0.0f, 0.0f, 0.0f, tx, ty, tx);
        cube.setTextureProperty(road_tex, road_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        f += 3.5;
    }
}

void fence(Cube& cube, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;
    float tz;

    //road strip2
    tz = 0;
    for (int i = 0; i < 60; i++)
    {
        model = transform(0+tz, 0, 0, 0.0f, 0.0f, 0.0f, .4, 4, .2);
        cube.setTextureProperty(road_tex, road_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        tz += 1;
    }
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

    float ty = 12.0f;
    float tz = .65f;

    //hollow cyllinder for stand
    model = transform(0, -178, 0, 0, 0, 0, .465, .05 + 100, .465);
    //curve_hollow_cyl.setTextureProperty(almari_tex, almari_tex, 1.0f);
    curve_hollow_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(1.0f, 0.0f, 0.0f));

    //stand notch
    model = transform(-.25f, 1.0 + ty, -.2f, 0.0f, 0.0f, 0.0f, .5, .35f, .85f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    
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
    wdz += .3;
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
    cube.setTextureProperty(marbel_tex, marbel_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //vertical surface l2
    model = transform(20.0f, 8.0f, 0.0f, 0.0f, 0.0f, 0.0f, 9.0f, .35f, 24.0f);
    cube.setTextureProperty(marbel_tex, marbel_tex, 32.0f);
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

    //polls
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    model = transform(0.0f, 0.0f, 8.0f-.3f, 0.0f, 0.0f, 0.0f, .3, 8.0f, .3f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    model = transform(11.0f, 0.0f, 8.0f-.3f, 0.0f, 0.0f, 0.0f, .3f, 8.0f, 0.3f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    model = transform(20.0f, 0.0f, 8.0f-.3f, 0.0f, 0.0f, 0.0f, .3f, 8.0f, 0.3f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    model = transform(20.0f, 0.0f, 24.0f-.3f, 0.0f, 0.0f, 0.0f, .3f, 8.0f, 0.3f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);


    //seats
    f = 0;
    for (int i = 0; i < 7; i++)
    {
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        model = transform(1.0f + f, 1.5, 2.0f, 0.0f, 0.0f, 0.0f, .6, .6, .6);
        seat(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
        //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        f += 4;
    }

    //seats
    f = 0;
    for (int i = 0; i < 4; i++)
    {
        cube.setTextureProperty(black_tex, black_tex, 32.0f);
        model = transform(1.0f + 7*4 - 2 , 1.5, 2.0f + 4+2+ f, 0.0f, -90, 0.0f, .6, .6, .6);
        seat(cube, lightingShader, lightingShaderWithTexture, alTogether * model);
        //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
        f += 4;
    }




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

void bus(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Cube& cube_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
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


    //bus floor
    model = transform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, p1 + p2 + p3 + p4 + p5, floor_y, floor_z + panel_z);
    cube.setTextureProperty(black_tex, black_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //bus ceiling
    model = transform(0.0f - front_x, 0.0f + front_y, 0.0f, 0.0f, 0.0f, 0.0f, p1 + p2 + p3 + p4 + p5 + (2 * front_x), panel_z, panel_y + (panel_z));
    //cube.setTextureProperty(black_tex, black_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    sq_tile_cube_pointer->setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    sq_tile_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);


    //bus front
    model = transform(0.0f - front_x, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, front_x, front_y, front_z + panel_z);
    //cube.setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    sq_tile_cube_pointer->setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    sq_tile_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //front window
    model = transform(0.0f - front_x - .1, 2.3f, 0.38f, 0.0f, 0.0f, 0.0f, front_x - .05, front_y / 2, front_z - .45);
    cube.setTextureProperty(door_tex, door_tex, 32.0f);
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
    //cube.setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    sq_tile_cube_pointer->setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    sq_tile_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);


    //back window
    model = transform(0.0f - front_x + floor_x + .45, 2.3f, 0.38f, 0.0f, 0.0f, 0.0f, front_x - .05, front_y / 2, front_z - .45);
    cube.setTextureProperty(door_tex, door_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);



    //bus back side(right side)
    //door angle
    if (bus_door_fl && bus_door_angle < 90)
    {
        bus_door_angle += .4f;
    }
    if (!bus_door_fl && bus_door_angle > 0.0)
    {
        bus_door_angle -= .4f;
    }

    //first part
    model = transform(0.0f, 0.0f + floor_y, 0.0f, 0.0f, 0.0f, 0.0f, p1, panel_y, panel_z);
    //cube.setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    sq_tile_cube_pointer->setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    sq_tile_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //front door
    model = transform(p1, 0.0f + floor_y, 0.0f, 0.0f, 0.0f + bus_door_angle, 0.0f, p2, panel_y, panel_z);
    cube.setTextureProperty(door_tex, door_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //middle
    model = transform(p1 + p2, 0.0f + floor_y, 0.0f, 0.0f, 0.0f, 0.0f, p3, panel_y, panel_z);
    //cube.setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    sq_tile_cube_pointer->setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    sq_tile_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //back door
    model = transform(p1 + p2 + p3, 0.0f + floor_y, 0.0f, 0.0f, 0.0f + bus_door_angle, 0.0f, p4, panel_y, panel_z);
    cube.setTextureProperty(door_tex, door_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //last part
    model = transform(p1 + p2 + p3 + p4, 0.0f + floor_y, 0.0f, 0.0f, 0.0f, 0.0f, p5, panel_y, panel_z);
    //cube.setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    sq_tile_cube_pointer->setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    sq_tile_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //back window1
    model = transform(p1 + p2 + .5, panel_y / 2.3 + floor_y, 0.0f - .1, 0.0f, 0.0f, 0.0f, front_z - 1.8, front_y / 2, front_x - .05);
    cube.setTextureProperty(window_tex, window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);

    //back window2
    model = transform(p1 + p2 + 4.2, panel_y / 2.3 + floor_y, 0.0f - .1, 0.0f, 0.0f, 0.0f, front_z - 1.8, front_y / 2, front_x - .05);
    cube.setTextureProperty(window_tex, window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);




    //bus front side(left side)
    //first part
    model = transform(0.0f, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f, 0.0f, p1, panel_y, panel_z);
    //cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    sq_tile_cube_pointer->setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    sq_tile_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    
    //front door
    model = transform(p1, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f - bus_door_angle, 0.0f, p2, panel_y, panel_z);
    cube.setTextureProperty(door_tex, door_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    
    //middle
    model = transform(p1 + p2, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f, 0.0f, p3, panel_y, panel_z);
    //cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    sq_tile_cube_pointer->setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    sq_tile_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //back door
    model = transform(p1 + p2 + p3, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f - bus_door_angle, 0.0f, p4, panel_y, panel_z);
    cube.setTextureProperty(door_tex, door_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    
    //last part
    model = transform(p1 + p2 + p3 + p4, 0.0f + floor_y, 0.0f + floor_z, 0.0f, 0.0f, 0.0f, p5, panel_y, panel_z);
    //cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);
    sq_tile_cube_pointer->setTextureProperty(decal_body_tex, decal_body_tex, 32.0f);
    sq_tile_cube_pointer->drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //front window1
    model = transform(p1 + p2 + .5, panel_y / 2.3 + floor_y, 0.0f + floor_z + .1, 0.0f, 0.0f, 0.0f, front_z - 1.8, front_y / 2, front_x - .05);
    cube.setTextureProperty(window_tex, window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether* model);

    //front window2
    model = transform(p1 + p2 + 4.2, panel_y / 2.3 + floor_y, 0.0f + floor_z + .1, 0.0f, 0.0f, 0.0f, front_z - 1.8, front_y / 2, front_x - .05);
    cube.setTextureProperty(window_tex, window_tex, 32.0f);
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

    curve_cyl.setTextureProperty(tyre_tex, tyre_tex, 1.0f);
    curve_hollow_cyl.setTextureProperty(tyre_tex, tyre_tex, 1.0f);


    //front wheel1
    model = transform(w_x, w_y, w_z, 90.0f, 0.0f, 0.0f, w_sx, w_sy, w_sz);
    Wheel_hollow(curve_cyl, curve_hollow_cyl, cube_cyl, lightingShader, lightingShaderWithTexture, alTogether* model);

    //front wheel2
    model = transform(w_x + 8.2f, w_y, w_z, 90.0f, 0.0f, 0.0f, w_sx, w_sy, w_sz);
    Wheel_hollow(curve_cyl, curve_hollow_cyl, cube_cyl, lightingShader, lightingShaderWithTexture, alTogether* model);

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
    //cube.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
}

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
    cube.drawCubeWithTexture(lightingShader, alTogether * model);
}