#include "ShapeManager.h"
#include "ScreenProperty.h"
#include "CollisionData.h"

ShapeManager::ShapeManager()
{
    this->basic2DShader = new Shader("2d_shape.vert", "2d_shape.frag");
    this->basic3DShader = new Shader("3d_shape.vert", "3d_shape.frag");
    
    this->camera = new Camera(glm::vec3(-0.3f, 0.2f, 1.0f));
    this->projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    this->selectedBoxIndex = 0;
    this->boxNumber = 0;
}

ShapeManager* ShapeManager::getInstance() {
    if (ShapeManager::drawingManager == nullptr) {
        ShapeManager::drawingManager = new ShapeManager();
    }
    return ShapeManager::drawingManager;
}

void ShapeManager::Destroy() {
   
    /*for (auto polygons : polygonsToRender) {
        delete polygons;
    }*/
  
    delete ShapeManager::drawingManager->camera;
    delete ShapeManager::drawingManager->basic2DShader;
    delete ShapeManager::drawingManager;
}

void ShapeManager::addBox(Box* box)
{
    boxes.push_back(box);
    this->boxNumber += 1;

}

void ShapeManager::renderAll()
{  
    // ���߿� �ð��� �� �����丵 �� ��
   
  
        this->basic3DShader->use();
        this->basic3DShader->setMat4("projection", projection);
        this->basic3DShader->setMat4("view", camera->GetViewMatrix());
        this->basic3DShader->setFloat("znear", 0.1);
        this->basic3DShader->setFloat("zfar", 100);

        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            cout << "Error";
        glEnable(GL_STENCIL_TEST);
        glDepthFunc(GL_LESS);
        glDepthRange(0.1, 100);
        
        for (int i = 0; i < this->boxNumber; i++) {
            glStencilFunc(GL_ALWAYS, i + 1, 1);

            boxes[i]->setShaderValue(this->basic3DShader);
            boxes[i]->render();
        }

        for (int i = 0; i < this->boxNumber; i++) {
            for (int j = i + 1; j < this->boxNumber; j++) {
                if (boxes[i]->isCollideWith(boxes[j])) {
                    cout << "i " << i << "j " <<  j << endl;
                    CollisionData* collisionData = new CollisionData(boxes[i], boxes[j]);
                }
            }
        }
}

void ShapeManager::selectThreeDimensionalFigure(int index)
{
    if (isValidIndex3d(index)) {
        this->selectedBoxIndex = index;
    }
}

void ShapeManager::processTranslation(float directionX, float directionY, float directionZ)
{
    if (this->selectedBoxIndex != -1) {
        boxes[this->selectedBoxIndex]->translation(directionX, directionY, directionZ);
    }
   
    
}

bool ShapeManager::isValidIndex3d(int index) {
    return index >= 0 && index < this->boxNumber;
}

void ShapeManager::rotateIn3D(glm::mat4 rogridMatrix)
{
    if (this->selectedBoxIndex != -1) {
        this->boxes[this->selectedBoxIndex]->transformation(rogridMatrix);
    }
}

void ShapeManager::moveCamera(float xoffset, float yoffset) {
    camera->ProcessMouseMovement(xoffset, yoffset);
}

void ShapeManager::processKeyBoard(CameraMovement direction, float deltaTime) {
    camera->ProcessKeyboard(direction, deltaTime);
}
ShapeManager* ShapeManager::drawingManager = nullptr;