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
#include <GL/glew.h>
#include "VAO.h"
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
/// @file VAO.cpp
/// @brief implementation files for VAO class
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
VAO::VAO(
                                                                             GLenum _mode
                                                                         )
{
    m_allocated=false;
    // first we create a vertex array Object
    //glGenVertexArrays(1, &m_id);
    /* use only vbo */
    m_id = genVBO();

    m_bound=false;
    m_drawMode=_mode;
    m_indicesCount=0;
    m_indexed=false;
    m_indexType = GL_UNSIGNED_BYTE;
}

//----------------------------------------------------------------------------------------------------------------------
//void VAO::bind()
//{
//    glBindVertexArray(m_id);
//    m_bound=true;
//}

void VAO::bindVBO(GLuint _vboID)
{
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);
    m_bound=true;
}

//----------------------------------------------------------------------------------------------------------------------
//void VAO::unbind()
//{
//    glBindVertexArray(0);
//  //glEnableVertexAttribArray(0);
//  //glBindBuffer(GL_ARRAY_BUFFER,0);

//    m_bound=false;
//}

void VAO::unbindVBO()
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_bound=false;
}

//----------------------------------------------------------------------------------------------------------------------
//void VAO::removeVOA()
//{
//    if(m_bound == true)
//    {
//        unbind();
//        unbindVBO();
//    }
//    if( m_allocated ==true)
//    {

//  int size=m_vboIDs.size();
//  for(int i=0; i<size; ++i)
//    glDeleteBuffers(1,&m_vboIDs[i]);

//    glDeleteVertexArrays(1,&m_id);
//    m_allocated=false;
//    }
//}

void VAO::removeVBO()
{
    int size=m_vboIDs.size();
    for(int i=0; i<size; ++i)
      glDeleteBuffers(1,&m_vboIDs[i]);

    m_allocated=false;
}

//----------------------------------------------------------------------------------------------------------------------
//void VAO::setData(
//                                 unsigned int _size,
//                                 const GLfloat &_data,
//                                 GLenum _mode
//                                )
//{
//    if(m_bound == false)
//    {
//        std::cerr<<"trying to set VOA data when unbound\n";
//    }
//    GLuint vboID;
//    glGenBuffers(1, &vboID);
//    m_vboIDs.push_back(vboID);
//    // now we will bind an array buffer to the first one and load the data for the verts
//    glBindBuffer(GL_ARRAY_BUFFER, vboID);
//    glBufferData(GL_ARRAY_BUFFER, _size, &_data, _mode);
//    m_allocated=true;

//}
//----------------------------------------------------------------------------------------------------------------------
void VAO::setData(
                                 GLuint _vboID,
                                 unsigned int _size,
                                 const GLfloat &_data,
                                 GLenum _mode
                                )
{
    if(m_bound == false)
    {
        std::cerr<<"trying to set VOA data when unbound\n";
    }

    // now we will bind an array buffer to the first one and load the data for the verts
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 64, BUFFER_OFFSET(0));

    glBufferData(GL_ARRAY_BUFFER, _size, &_data, _mode) ;
    glGetError();
//    glDisableClientState(GL_VERTEX_ARRAY);
    m_allocated=true;

}
//----------------------------------------------------------------------------------------------------------------------
GLuint VAO::genVBO()
{
    GLuint vboID;
    glGenBuffers(1, &vboID);
    m_vboIDs.push_back(vboID);
    return vboID;
}


int VAO::getVBOsize()
{ /*    http://stackoverflow.com/questions/3819390/how-do-i-get-the-size-of-a-vbo-in-opengl-es       */
    if(m_bound !=true)
    {
        std::cerr<<"Warning trying to get VBO size on unbound VOA\n";
    }
    int nBufferSize = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &nBufferSize);
    return nBufferSize;
}

void VAO::setSubData(int _offset,
                     int _size,
                     const GLfloat &_data)
{
    if(m_bound !=true)
    {
        std::cerr<<"Warning trying to set subdata on unbound VOA\n";
    }

    int MAX;
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &MAX);
    glBufferSubData(GL_ARRAY_BUFFER, _offset, _size, &_data);
}


//void VAO::setIndexedData(
//                                        unsigned int _size,
//                                        const GLfloat &_data,
//                                        unsigned int _indexSize,
//                                        const GLubyte &_indexData,
//                                        GLenum _mode
//                                        )
//{
	
//    if(m_bound == false)
//    {
//        std::cerr<<"trying to set VOA data when unbound\n";
//    }
//    GLuint vboID;
//    glGenBuffers(1, &vboID);
//    m_vboIDs.push_back(vboID);

//    GLuint iboID;
//    glGenBuffers(1, &iboID);
//    m_iboID = iboID;

//    //glEnableVertexAttribArray(0);
//    // now we will bind an array buffer to the first one and load the data for the verts
//    glBindBuffer(GL_ARRAY_BUFFER, vboID);
//    glBufferData(GL_ARRAY_BUFFER, _size, &_data, _mode);

//    // now for the indices
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexSize * sizeof(GLubyte), &_indexData, _mode);

//    m_allocated=true;
//    m_indexed=true;
//    m_indexType=GL_UNSIGNED_BYTE;
//}

//void VAO::setIndexedData(
//                                        unsigned int _size,
//                                        const GLfloat &_data,
//                                        unsigned int _indexSize,
//                                        const GLushort &_indexData,
//                                        GLenum _indexType,
//                                        GLenum _mode
//                                        )
//{
		
//    if(m_bound == false)
//    {
//        std::cerr<<"trying to set VOA data when unbound\n";
//    }
//    GLuint vboID;
//    glGenBuffers(1, &vboID);
//    m_vboIDs.push_back(vboID);

//    GLuint iboID;
//    glGenBuffers(1, &iboID);
//    m_iboID = iboID;
	
//    //glEnableVertexAttribArray(0);
//    // now we will bind an array buffer to the first one and load the data for the verts
//    glBindBuffer(GL_ARRAY_BUFFER, vboID);
//    glBufferData(GL_ARRAY_BUFFER, _size, &_data, _mode);

//    // now for the indices
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexSize * sizeof(_indexType), &_indexData, _mode);

//    m_allocated=true;
//    m_indexed=true;
//    m_indexType=_indexType;
//}

//void VAO::updateIndexedData(
//                                            unsigned int _size,
//                                            const GLfloat &_data,
//                                            GLenum _mode
//                                         )
//{
		
//    if(m_bound == false)
//    {
//        std::cerr<<"trying to set VOA data when unbound\n";
//    }
	
//    if(m_allocated && m_indexed)
//    {
//        glBindBuffer(GL_ARRAY_BUFFER, getVBOid(0));
//        glBufferData(GL_ARRAY_BUFFER, _size, &_data, _mode);
//    }
//}




GLuint VAO::getVBOid(unsigned int _index)
{
  GLuint id=0;
  /// _index >=0 && picked up by g++4.3 so removed from below
  if(_index<m_vboIDs.size())
  {
    id=m_vboIDs[_index];
  }
  return id;
}

//----------------------------------------------------------------------------------------------------------------------
void VAO::setVertexAttributePointer(
                                        GLuint _id,
                                        GLint _size,
                                        GLenum _type,
                                        GLsizei _stride,
                                        unsigned int _dataOffset,
                                        bool _normalise
                                    )
{
    if(m_bound !=true)
    {
        std::cerr<<"Warning trying to set attribute on Unbound VOA\n";
    }

    glVertexAttribPointer(_id,_size,_type,_normalise,_stride,((float *)NULL + (_dataOffset)));
    glEnableVertexAttribArray(_id);
}
//----------------------------------------------------------------------------------------------------------------------
void VAO::draw() const
{
    if(m_allocated == false)
    {
        std::cerr<<"Warning trying to draw an unallocated VOA\n";
    }
    if(m_bound == false)
    {
        std::cerr<<"Warning trying to draw an unbound VOA\n";
    }

    if(m_indexed == false)
    {
        glDrawArrays(m_drawMode, 0, m_indicesCount);	// draw first object
    }
    else
    {
        glDrawElements(m_drawMode,m_indicesCount,m_indexType,(GLvoid*)((char*)NULL));
    }
}

void VAO::draw(GLenum _mode) const
{
  if(m_allocated == false)
  {
    std::cerr<<"Warning trying to draw an unallocated VOA\n";
  }
  if(m_bound == false)
  {
    std::cerr<<"Warning trying to draw an unbound VOA\n";
  }

  if(m_indexed == false)
  {
    glDrawArrays(_mode, 0, m_indicesCount);	// draw first object
  }
  else
  {
    glDrawElements(m_drawMode,m_indicesCount,m_indexType,(GLvoid*)((char*)NULL));
  }
}


//----------------------------------------------------------------------------------------------------------------------
ngl::Real *VAO::getDataPointer(unsigned int _vbo)
{
  ngl::Real *ptr=0;
//  code was this but g++ 4.2 gives warning about it always being true
// removed the first test as change to uint so should be ok
//  if(_vbo >=0 && _vbo<m_vboIDs.size())
//
  if(_vbo<m_vboIDs.size())
  {
    std::cout<<"binding vao to get data ptr ";
    //bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIDs[_vbo]);
    //NGLCheckGLError("Abstract Mesh",__LINE__);
    ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    std::cout<<ptr<<"\n";
  }
  return ptr;
}




