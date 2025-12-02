
class Canal {

private: 

    int id;        
    int ccNumber;  
    int value;

public:
    Canal();
    Canal(int idcanal, int cc, int valor);
    ~Canal();

   
    int getId() const;
    int getCcNumber() const;
    int getValue() const;

    
    void setValue(int v);

    
};





