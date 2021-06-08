//
//  IntArray.hpp
//  01_push_box_xcode
//
//  Created by huangwei on 2021/6/8.
//

#ifndef IntArray_hpp
#define IntArray_hpp

class IntArray2D {
public:
    IntArray2D(int size0, int size1): mArray{ nullptr }, mSize0{ size0 }, mSize1{ size1 } {
        mArray = new int[ size0 * size1 ];
    }
    
    ~IntArray2D() { delete[] mArray; mArray = nullptr; }
    
    int& operator()( int index0, int index1 ) {
        return mArray[index1 * mSize0 + index0];
    }
    
    const int& operator()( int index0, int index1 ) const {
        return mArray[index1 * mSize0 + index0];
    }
private:
    int* mArray;
    const int mSize0;
    const int mSize1;
};


#endif /* IntArray_hpp */
