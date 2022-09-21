#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>// 싱글 헤더 이미지로드 라이브러리

#include <glm/glm.hpp> // GLM 기본 헤더, 여러 자료형 포함
#include <glm/gtc/matrix_transform.hpp>// 행렬 변환에 사용되는 함수 모음
#include <glm/gtc/type_ptr.hpp>// GLM 자료형들의 메모리 관련 기능

#include <SHADERS/shader.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfwInit : GLFW 초기화
    // glfwWindowHint : LFW 설정 (파라미터 1 : 우리가 설정하고자 하는 옵션 알려줌, GLFW_ 접두어 선택 가능
    // 2 : 옵션 값 설정, 설정가능한 옵션과 값은 문서 참고하기 !, Mac OS는 더 추가해야함  )
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // window 객체 생성, 이 window 객체는 모든 window 데이터 보유
    // GLFWwindow는 처음 두개의 파라미터로 창의 너비와 높이 받음, 직접 숫자 넣어도 되고 따로 변수 가능(아래와 같음)
    // 세번째는 창 이름
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SONGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 현재 윈도우의 context를 현재 스레드의 주 context로 설정
    glfwMakeContextCurrent(window);
    // buffer의 크기가 변경될 때 호출되는 콜백
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // glad 초기화
    // GLFW는 os에 따라 올바른 함수를 정의하는 glfwGetProcAddress 제공
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 활성화
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // configure global opengl state
    //glEnable(GL_DEPTH_TEST); // 위에 추가하면서 비활성화

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("test2.vs", "test2.fs");

    // vertices 배열에 float타입 컬러 데이터 추가
    // = 텍스처 좌표를 vertex 데이터에 추가
    float vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left  
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    // vertex attribute를 추가했기 때문에 다시 알려줘야함
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    GLuint texture1[2];

    // texture 1
    glGenTextures(2, texture1);
    glBindTexture(GL_TEXTURE_2D, texture1[0]);

    // 텍스처 랩핑
    // 파라미터 1 : 텍스처 타겟 지정
    // 파라미터 2 : 설정할 옵션, 어떤 축에 적용할 것인지 지정
    // 파라미터 3 : 텍스처 wrapping 모드 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 텍스처 필터링
    // GL_LINEAR는 GL_NEAREST보다 좀 더 경계가 불분명 -> 매끄러움
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 

    // 이미지 로드
    // 파라미터 1 : 이미지 파일 경로 받기
    // 파라미터 2, 3, 4 : 이미지의 너비, 높이, 컬러 채널 수(전부 정수형 변수)
    unsigned char* data = stbi_load("textures/brick.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // 파라미터 1 : 텍스처 타겟 지정
        // 파라미터 2 : 텍스처 미니맵 레벨을 수동으로 지정하고 싶을 때 사용
        // 파라미터 3 : 우리가 저장하고 싶은 텍스처가 어떤 포맷을 가져야 할 지, 여기서는 RGB값만 사용
        // 파라미터 4, 5 : 텍스처의 너비와 높이 설정
        // 파라미터 6 : 항상 0 지정해야함
        // 파리미터 7, 8 : 원본이미지의 포멧과 데이터 타입 지정
        // 파라미터 9 : 실제 이미지 데이터
        // 현재는 베이스 레벨, 만약 미니맵 사용하고 싶다면 모든 이미지 직접 지정 후 아래 함수 사용
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    
    // texture 2
    GLuint texture2;
    //glGenTextures(1, texture1[1]);
    glBindTexture(GL_TEXTURE_2D, texture1[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data2 = stbi_load("textures/brick2.png", &width, &height, &nrChannels, 0);
    if (data2)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    stbi_image_free(data2);
    
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);


    // 메인 루프(render loop) : 우리가 그만하라고 할때까지 계속 실행
    // glfwWindowShouldClose : 각 루프 시작때마다 GLFW가 종료하도록 지시되었는지 확인
    // 만약 그렇다면 true반환해서 중지 
    //모든 렌더링 명령은 렌더링 루프 안에
    while (!glfwWindowShouldClose(window))
    {
        // input 처리
        processInput(window);

        // buffer 초기화
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// 화면의 컬러 버퍼 지우기, 어떤 색으로 지울지 설정(상태 설정 함수)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //사용 가능 비트 : color, depth, stencil, 일단 컬러값만 생각해서 그것만 지움(상태 사용 함수) 

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1[1]);

        // activate shader
        ourShader.use();

        // glm::perspective : 눈에 보이는 공간을 정의하는 균일하지 않은 상자모양(절도체) 생성
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        // model 행렬 : vertex들을 world space로 변환하기 위한 것들로 이루어져 있음
        // vertex * model -> vertex를 world좌표로 변환 가능
        model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));// 이부분 수정, 회전 방지 입력
        // 우리가 움직이고 싶은 방향과 반대로 scene 이동
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        // 파라미터 1 : fov 값 지정(view space가 얼마나 큰지, 보통 현실적 시점을 위해 45도로 설정)
        // 파라미터 2 : viewport의 너비를 높이로 나눔으로써 계산되는 화면 비율 설정
        // 파라미터 3, 4 : 가까운 평면과 먼 평먼 사이 거리 설정
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        // render box
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glfwSwapBuffers(window);// 컬러버퍼(반복 중 이미지 그리고 화면에 출력하는 기능) 교체
        glfwPollEvents();// 이벤트 발생 확인 -> 윈도우 상태 업데이트, 정해진 함수 호출
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();// 렌더링 루프 종료되면 할당되었던 모든 자원 정리/삭제
    return 0;
}

//입력 관리
//glfwGetKey : 키보드 키와 함께 윈도우 입력 받을 수 있음, 해당 키가 눌려져 있는지 여부 리턴
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//ESC 눌렀는지 확인 -> 눌렀으면 true->GLFW 닫음
        glfwSetWindowShouldClose(window, true);
}

// OpenGL에게 렌더링 윈도우 사이즈 알려줌
// glViewport 통해서 차원 설정 가능
// 1, 2 파라미터는 윈도우 왼쪽 아래 모서리 위치 설정, 3, 4 파라미터는 렌더링 윈도우 너비와 높이 픽셀지정 (GLFW 자체에서 가져옴)
// 내부에서 OpenGL은 glViewport 함수 통해 지정된 데이터 사용해서 2D->스크린 좌표로 변환
// 창의 크기 조정하는 순간 뷰포트도 조정 필요 -> 이게 framebuffer_size_callback
// f_b_s 함수는 파라미터 1로 GLFWwindow 객체 받고, 2, 3 파라미터로 창의 새로운 크기 나타내는 두개의 정수 받음
// 창의 크기 변경될때마다 GLFW는 이 함수 호출 -> 적절한 데이터 채우기
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
