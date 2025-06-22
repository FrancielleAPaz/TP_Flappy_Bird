class GameObject {
protected:
    float x;  // Posição X no cenário
    float y;  // Posição Y no cenário

public:
    GameObject(float startX, float startY);
    virtual ~GameObject() = default;

    // Métodos virtuais puros (interface polimórfica)
    virtual void update() = 0;  // Atualiza o estado do objeto
    virtual void draw(ALLEGRO_DISPLAY* display) const = 0;  // Renderiza o objeto
    virtual bool checkCollision(const GameObject& other) const = 0;  // Verifica colisão
    
    // Métodos concretos para acesso às propriedades
    float getX() const;
    float getY() const;
    
};
