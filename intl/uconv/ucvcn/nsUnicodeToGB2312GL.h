/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is Netscape Communications
 * Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 *            Yueheng Xu, yueheng.xu@intel.com
 */

#ifndef nsUnicodeToGB2312GL_h___
#define nsUnicodeToGB2312GL_h___

#include "nsUCvCnSupport.h"
#include "gbku.h"
//----------------------------------------------------------------------
// Class nsUnicodeToGB2312GL [declaration]

/**
 * A character set converter from Unicode to GB2312GL.
 *
 * @created         06/Apr/1999
 * @author  Catalin Rotaru [CATA]
 * @Modified to use the newer tables in file gbku.h.   
 *  Yueheng.xu@intel.com. 3/May/2000
 */

class nsUnicodeToGB2312GL : public nsEncoderSupport
{
public:

  nsUnicodeToGB2312GL();

protected:

  NS_IMETHOD ConvertNoBuff(const PRUnichar * aSrc, 
                            PRInt32 * aSrcLength, 
                            char * aDest, 
                            PRInt32 * aDestLength);

  //--------------------------------------------------------------------
  // Subclassing of nsEncoderSupport class [declaration]

  NS_IMETHOD GetMaxLength(const PRUnichar * aSrc, PRInt32 aSrcLength, 
      PRInt32 * aDestLength);

  NS_IMETHOD ConvertNoBuffNoErr(const PRUnichar * aSrc, PRInt32 * aSrcLength, 
                                char * aDest, PRInt32 * aDestLength)
  {
    return NS_OK;
  };  // just make it not abstract;

  NS_IMETHOD FillInfo(PRUint32 *aInfo);

};


#endif /* nsUnicodeToGB2312GL_h___ */
