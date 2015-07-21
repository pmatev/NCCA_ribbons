/*
  Copyright (C) 2009 Jon Macey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __VAO_H__
#define __VAO_H__
//----------------------------------------------------------------------------------------------------------------------
/// @file VAO.h
/// @brief a class to store an OpenGL VAO
//----------------------------------------------------------------------------------------------------------------------
// part of the python bindings
#ifdef NO_PYTHON_LIB
  #include <boost/python.hpp>
  #include <boost/format.hpp>
#endif

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

// must include types.h first for ngl::Real and GLEW if required
#include <ngl/Util.h>
#include <ngl/Types.h>


//----------------------------------------------------------------------------------------------------------------------
/// @file VAO.h
/// @brief encapsulates an OpenGL VAO
/// @author Jonathan Macey (Modified by Peter Matev)
/// @version 3.0
/// @date 14/6/11 Initial version
/// Revision History :
/// Attempted to create more functions for managing VBO's. /* Unfinished */
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// @class VAO "include/VAO.h"
/// @brief a class to encapsulate an OpenGL VAO with extensions
/// for VBO's and dynamic topology
//----------------------------------------------------------------------------------------------------------------------
class VAO
{

public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to return a newVOA object
    /// @returns a new VOA object
    //----------------------------------------------------------------------------------------------------------------------
    inline static VAO * createVOA(
                                                    GLenum _mode
                                                )
                                                 {
                                                     return new VAO(_mode);
                                                 }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief bind this VOA to make it active
    //----------------------------------------------------------------------------------------------------------------------
    void bind();


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief bind VBO with given id
    //----------------------------------------------------------------------------------------------------------------------
    void bindVBO(GLuint _vboID);


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief unbind the VOA and set 0 to be the active VOA
    //----------------------------------------------------------------------------------------------------------------------
    void unbind();


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief unbind the active VBO
    //----------------------------------------------------------------------------------------------------------------------
    void unbindVBO();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief delete the VOA (using OpenGL calls to clear data etc)
    //----------------------------------------------------------------------------------------------------------------------
    void removeVOA();


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief remove VBO
    //----------------------------------------------------------------------------------------------------------------------
    void removeVBO();


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief accesor for the VOA id
    /// @brief returns the VOA id
    //----------------------------------------------------------------------------------------------------------------------
    inline GLuint getID()const {return m_id;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief accesor to see if VOA is bound
    /// @brief returns the bind state (true for bound)
    //----------------------------------------------------------------------------------------------------------------------
    inline bool isBound()const {return m_bound;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief accesor to see if VOA has allocated data
    /// @brief returns the allocated state (true for allocated)
    //----------------------------------------------------------------------------------------------------------------------
    inline bool isAllocated()const {return m_allocated;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief allocate our data using raw face values (for example tri's) data, attributes must be bound to match at
    /// a different level of code (usually in the client as part of the shader loading, see VAO examples for more details
    /// @param _size the size of the raw data passed
    /// @param _data the actual data to set for the VOA
    /// @param _mode the draw mode hint used by GL
    //----------------------------------------------------------------------------------------------------------------------
    void setData(
                    unsigned int _size,
                    const GLfloat &_data,
                    GLenum _mode=GL_STATIC_DRAW
                );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set data to a specific vbo of given id
    //----------------------------------------------------------------------------------------------------------------------
    void setData(
                     GLuint _vboID,
                     unsigned int _size,
                     const GLfloat &_data,
                     GLenum _mode=GL_STATIC_DRAW
                );


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief generate a new VBO and return the id
    //----------------------------------------------------------------------------------------------------------------------
    GLuint genVBO();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set sub data
    //----------------------------------------------------------------------------------------------------------------------
    void setSubData(int _offset,
                    int _size,
                    const GLfloat &_data);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief return size of VBO in bytes
    //----------------------------------------------------------------------------------------------------------------------
    int getVBOsize();

    /// @brief allocate our data using raw face values (for example tri's) data, attributes must be bound to match at
    /// a different level of code (usually in the client as part of the shader loading, see VAO examples for more details
    /// This value uses an index array to point to series of data to store
    /// @param _size the size of the raw data passed
    /// @param _data the actual data to set for the VOA
    /// @param _indexSize the size of the index array passed
    /// @param _indexData the actual data to set for the VOA indexes (only GLubyte data at present need to write more methods
    /// but usually only use this
    /// @param _mode the draw mode hint used by GL
    //----------------------------------------------------------------------------------------------------------------------
    void setIndexedData(
                                        unsigned int _size,
                                        const GLfloat &_data,
                                        unsigned int _indexSize,
                                        const GLubyte &_indexData,
                                        GLenum _mode=GL_STATIC_DRAW
	
                                     );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief allocate our data using raw face values (for example tri's) data, attributes must be bound to match at
    /// a different level of code (usually in the client as part of the shader loading, see VAO examples for more details
    /// This value uses an index array to point to series of data to store
    /// @param _size the size of the raw data passed
    /// @param _data the actual data to set for the VOA
    /// @param _indexSize the size of the index array passed
    /// @param _indexData the actual data to set for the VOA indexes (only GLubyte data at present need to write more methods
    /// but usually only use this
    /// @param _indexType the type of the values in the indices buffer. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
    /// @param _mode the draw mode hint used by GL
    //----------------------------------------------------------------------------------------------------------------------
    void setIndexedData(
                                        unsigned int _size,
                                        const GLfloat &_data,
                                        unsigned int _indexSize,
                                        const GLushort &_indexData,
                                        GLenum _indexType,
                                        GLenum _mode=GL_STATIC_DRAW
                                     );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief allow to update the vertex data of the VBO when declared as dynamic (with GL_STREAM_DRAW)
    /// @param _size the size of the raw data passed
    /// @param _data the actual data to set for the VOA
    /// @param _mode the draw mode hint used by GL (choose GL_STREAM_DRAW by default, recommended mode for dynamic VBO's)
    //----------------------------------------------------------------------------------------------------------------------
	
    void updateIndexedData(
                                    unsigned int _size,
                                    const GLfloat &_data,
                                    GLenum _mode=GL_STREAM_DRAW
                                 );
	
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief allocate our data
    /// @param _size the size of the raw data passed (not counting sizeof(GL_FLOAT))
    /// @param _type the data type of the Pointer (eg GL_FLOAT)
    /// @param _stride the step between data values (need *sizeof _type) e.g. 10*sizeof(GLfloat)
    /// @param _dataOffset Specifies a pointer to the first component of the first generic vertex attribute in the array. The initial value is 0. and is calculated as ((float *)NULL + (_dataOffset))
    /// @param _normalise specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as
    /// fixed-point values
    /// (GL_FALSE) when they are accessed.
    //----------------------------------------------------------------------------------------------------------------------
    void setVertexAttributePointer(
                                                                GLuint _id,
                                                                GLint _size,
                                                                GLenum _type,
                                                                GLsizei _stride,
                                                                unsigned int _dataOffset,
                                                                bool _normalise=GL_FALSE
                                                            );
    /// @brief get the VBO id for the data mapped at index _index
    /// basically this will be the vbo for the setData called, so if it has been called
    /// 3 times 0 would be first call 1, 2nd etc.
    /// @param[in] _index the index to get the id for
    /// @returns 0 if not found else the vbo id
    GLuint getVBOid(unsigned int _index );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief draw the VOA
    //----------------------------------------------------------------------------------------------------------------------
    void draw() const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief draw the VOA
  /// @param[in] _mode the mode to draw with
  //----------------------------------------------------------------------------------------------------------------------
  void draw(GLenum _mode) const;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the number of faces to draw
    /// @param _n the number of indices to draw in glDrawArray (param 3 count)
    //----------------------------------------------------------------------------------------------------------------------
    inline void setNumIndices(GLuint _n){m_indicesCount=_n;}
  ngl::Real *getDataPointer(unsigned int _vbo);
  ~VAO(){;}

protected :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the draw mode of the VAO e.g. GL_TRIANGLES
    //----------------------------------------------------------------------------------------------------------------------
    GLenum m_drawMode;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the number of faces to draw
    //----------------------------------------------------------------------------------------------------------------------
    GLenum m_indicesCount;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the id of the VAO allocated by OpenGL
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_id;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if we have allocated the data to the VAO
    //----------------------------------------------------------------------------------------------------------------------
    bool m_allocated;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if we have an index or non index vao to draw
    //----------------------------------------------------------------------------------------------------------------------
    bool m_indexed;
	
    /// @brief Specifies the type of the values in the indices buffer. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or
    //  GL_UNSIGNED_INT.
    //----------------------------------------------------------------------------------------------------------------------
    GLenum m_indexType;
	
	
    GLuint m_iboID;
	
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if we are bound
    //----------------------------------------------------------------------------------------------------------------------
    bool m_bound;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief an array of VBO associated with this VAO
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <GLuint> m_vboIDs;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief hide the ctor as we want to create static factory only
  //----------------------------------------------------------------------------------------------------------------------
  VAO(){;}
  //----------------------------------------------------------------------------------------------------------------------
    /// @brief hide the ctor as we want to create static factory only
    //----------------------------------------------------------------------------------------------------------------------
    VAO(GLenum _mode);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief hide the copy ctor as we want to create static factory only
    //----------------------------------------------------------------------------------------------------------------------
    VAO(const VAO &_v);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief hide the dtor as we want to create static factory only
    //----------------------------------------------------------------------------------------------------------------------

};



#endif

//----------------------------------------------------------------------------------------------------------------------
