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
 */
#ifndef nsBrowserWindow_h___
#define nsBrowserWindow_h___

// Local Includes
#include "nsWebBrowserChrome.h"


#include "nsIBrowserWindow.h"
#include "nsIStreamListener.h"
#include "nsIProgressEventSink.h"
#include "nsIWebShell.h"
#include "nsIDocShell.h"
#include "nsIDocumentLoaderObserver.h"
#include "nsString.h"
#include "nsVoidArray.h"
#include "nsCRT.h"
#include "prtime.h"
#include "nsFileSpec.h"

#include "nsIXPBaseWindow.h"
#include "nsPrintSetupDialog.h"
#include "nsFindDialog.h"
#include "nsTableInspectorDialog.h"
#include "nsImageInspectorDialog.h"
#include "nsIPrompt.h"

class nsILabel;
class nsICheckButton;
class nsIRadioButton;
class nsITextWidget;
class nsIButton;
class nsThrobber;
class nsViewerApp;
class nsIDocumentViewer;
class nsIPresContext;
class nsIPresShell;
class nsIPref;
class nsIContentConnector;
class nsWebCrawler;

#define SAMPLES_BASE_URL "resource:/res/samples"

/**
 * Abstract base class for our test app's browser windows
 */
class nsBrowserWindow : public nsIBrowserWindow,
                        public nsIDocumentLoaderObserver,
                        public nsIProgressEventSink,
                        public nsIWebShellContainer,
                        public nsIPrompt
{
friend class nsWebBrowserChrome;

public:
  NS_DECL_AND_IMPL_ZEROING_OPERATOR_NEW

  // nsISupports
  NS_DECL_ISUPPORTS

  // nsIBrowserWindow
  NS_IMETHOD Init(nsIAppShell* aAppShell,
                  nsIPref* aPrefs,
                  const nsRect& aBounds,
                  PRUint32 aChromeMask,
                  PRBool aAllowPlugins = PR_TRUE);
                  
  // virtual method for platform-specific initialization
  NS_IMETHOD InitNativeWindow() { return NS_OK; }
  
  // nsIBrowserWindow
  NS_IMETHOD MoveTo(PRInt32 aX, PRInt32 aY);
  NS_IMETHOD SizeContentTo(PRInt32 aWidth, PRInt32 aHeight);
  NS_IMETHOD SizeWindowTo(PRInt32 aWidth, PRInt32 aHeight,
                          PRBool aWidthTransient, PRBool aHeightTransient);
  NS_IMETHOD GetContentBounds(nsRect& aBounds);
  NS_IMETHOD GetWindowBounds(nsRect& aBounds);
  NS_IMETHOD IsIntrinsicallySized(PRBool& aResult);
  NS_IMETHOD ShowAfterCreation() { return Show(); }
  NS_IMETHOD Show();
  NS_IMETHOD Hide();
  NS_IMETHOD Close();
  NS_IMETHOD ShowModally(PRBool aPrepare);
  NS_IMETHOD SetChrome(PRUint32 aNewChromeMask);
  NS_IMETHOD GetChrome(PRUint32& aChromeMaskResult);
  NS_IMETHOD SetTitle(const PRUnichar* aTitle);
  NS_IMETHOD GetTitle(PRUnichar** aResult);
  NS_IMETHOD SetStatus(const PRUnichar* aStatus);
  NS_IMETHOD GetStatus(const PRUnichar** aResult);
  NS_IMETHOD SetDefaultStatus(const PRUnichar* aStatus);
  NS_IMETHOD GetDefaultStatus(const PRUnichar** aResult);
  NS_IMETHOD SetProgress(PRInt32 aProgress, PRInt32 aProgressMax);
  NS_IMETHOD ShowMenuBar(PRBool aShow);
  NS_IMETHOD GetWebShell(nsIWebShell*& aResult);
  NS_IMETHOD GetContentWebShell(nsIWebShell **aResult);

  // nsIDocumentLoaderObserver
  NS_IMETHOD OnStartDocumentLoad(nsIDocumentLoader* loader, 
                                 nsIURI* aURL, 
                                 const char* aCommand);
  NS_IMETHOD OnEndDocumentLoad(nsIDocumentLoader* loader,
                               nsIChannel* channel,
                               nsresult aStatus);
  NS_IMETHOD OnStartURLLoad(nsIDocumentLoader* loader,
                            nsIChannel* channel);
  NS_IMETHOD OnProgressURLLoad(nsIDocumentLoader* loader,
                               nsIChannel* channel,
                               PRUint32 aProgress, 
                               PRUint32 aProgressMax);
  NS_IMETHOD OnStatusURLLoad(nsIDocumentLoader* loader,
                             nsIChannel* channel,
                             nsString& aMsg);
  NS_IMETHOD OnEndURLLoad(nsIDocumentLoader* loader,
                          nsIChannel* channel,
                          nsresult aStatus);
  NS_IMETHOD HandleUnknownContentType( nsIDocumentLoader* loader,
                                       nsIChannel* channel,
                                       const char *aContentType,
                                       const char *aCommand );

  // nsIProgressEventSink
  NS_DECL_NSIPROGRESSEVENTSINK

  // nsIWebShellContainer
  NS_IMETHOD WillLoadURL(nsIWebShell* aShell, const PRUnichar* aURL, nsLoadType aReason);
  NS_IMETHOD BeginLoadURL(nsIWebShell* aShell, const PRUnichar* aURL);
  NS_IMETHOD ProgressLoadURL(nsIWebShell* aShell, const PRUnichar* aURL, PRInt32 aProgress, PRInt32 aProgressMax);
  NS_IMETHOD EndLoadURL(nsIWebShell* aShell, const PRUnichar* aURL, nsresult aStatus);
  NS_IMETHOD NewWebShell(PRUint32 aChromeMask,
                         PRBool aVisible,
                         nsIWebShell *&aNewWebShell);
  NS_IMETHOD ContentShellAdded(nsIWebShell* aChildShell, nsIContent* frameNode);
  NS_IMETHOD FindWebShellWithName(const PRUnichar* aName, nsIWebShell*& aResult);
  NS_IMETHOD FocusAvailable(nsIWebShell* aFocusedWebShell, PRBool& aFocusTaken);

  // nsIPrompt
  NS_DECL_NSIPROMPT

  // nsBrowserWindow
  void SetWebCrawler(nsWebCrawler* aWebCrawler);
  virtual nsresult CreateMenuBar(PRInt32 aWidth) = 0;
  virtual nsresult GetMenuBarHeight(PRInt32 * aHeightOut);
  virtual nsresult CreateToolBar(PRInt32 aWidth);
  virtual nsresult CreateStatusBar(PRInt32 aWidth);
  void Layout(PRInt32 aWidth, PRInt32 aHeight);
  void Back();
  void Forward();
  void GoTo(const PRUnichar* aURL,const char* aCommand=nsnull);
  void StartThrobber();
  void StopThrobber();
  void LoadThrobberImages();
  void DestroyThrobberImages();
  virtual nsEventStatus DispatchMenuItem(PRInt32 aID) = 0;

  void DoFileOpen();
  void DoCopy();
  void DoPaste();
  void DoJSConsole();
  void DoPrefs();
  void DoEditorMode(nsIDocShell* aDocShell);
  void DoEditorTest(nsIDocShell* aDocShell, PRInt32 aCommandID);
  nsIPresShell* GetPresShell();

  void DoFind();
  NS_IMETHOD FindNext(const nsString &aSearchStr, PRBool aMatchCase, PRBool aSearchDown, PRBool &aIsFound);
  NS_IMETHOD ForceRefresh();

  void DoTreeView();
  void DoToolbarDemo();

  void ShowPrintPreview(PRInt32 aID);
  void DoPrint(void);
  void DoPrintSetup(void);
  void DoTableInspector(void);
  void DoImageInspector(void);

#ifdef NS_DEBUG
  void DumpContent(FILE *out = stdout);
  void DumpFrames(FILE *out = stdout, nsString *aFilter = nsnull);
  void DumpViews(FILE *out = stdout);
  void DumpWebShells(FILE *out = stdout);
  void DumpStyleSheets(FILE *out = stdout);
  void DumpStyleContexts(FILE *out = stdout);
  void ToggleFrameBorders();
  void ToggleBoolPrefAndRefresh(const char * aPrefName);
  void ShowContentSize(FILE* out = stdout);
  void ShowFrameSize(FILE* out = stdout);
  void ShowStyleSize();
  void DoDebugSave();
  void DoToggleSelection();
  void DoDebugRobot();
  void DoSiteWalker();
  nsEventStatus DispatchDebugMenu(PRInt32 aID);
#endif
  nsEventStatus DispatchStyleMenu(PRInt32 aID);
  void SetCompatibilityMode(PRBool aIsStandard);

  nsEventStatus ProcessDialogEvent(nsGUIEvent *aEvent);

  // Initialize a second view on a different browser windows document
  // viewer.
  nsresult Init(nsIAppShell* aAppShell,
                nsIPref* aPrefs,
                const nsRect& aBounds,
                PRUint32 aChromeMask,
                PRBool aAllowPlugins,
                nsIDocumentViewer* aDocViewer,
                nsIPresContext* aPresContext);

  void SetApp(nsViewerApp* aApp) {
    mApp = aApp;
  }

  static void CloseAllWindows();

  void SetShowLoadTimes(PRBool aOn) {
    mShowLoadTimes = aOn;
  }

  nsViewerApp* mApp;
  nsWebCrawler* mWebCrawler;

  PRUint32 mChromeMask;
  nsString mTitle;

  nsIWidget* mWindow;
  nsIDocShell* mDocShell;

  nsFileSpec mOpenFileDirectory;

  PRTime mLoadStartTime;
  PRBool mShowLoadTimes;

  // "Toolbar"
  nsITextWidget* mLocation;
  nsIButton* mBack;
  nsIButton* mForward;
  nsThrobber* mThrobber;
  
  // "Status bar"
  nsITextWidget* mStatus;

  // Find Dialog
  nsIBrowserWindow * mDialog;
  nsIButton      * mCancelBtn;
  nsIButton      * mFindBtn;
  nsITextWidget  * mTextField;
  nsICheckButton * mMatchCheckBtn;
  nsIRadioButton * mUpRadioBtn;
  nsIRadioButton * mDwnRadioBtn;
  nsILabel       * mLabel;

  nsIXPBaseWindow * mXPDialog;
  nsIXPBaseWindow * mTableInspectorDialog;
  nsIXPBaseWindow * mImageInspectorDialog;
  PrintSetupInfo    mPrintSetupInfo;

  nsTableInspectorDialog * mTableInspector;
  nsImageInspectorDialog * mImageInspector;

  //for creating more instances
  nsIAppShell* mAppShell;
  nsIPref* mPrefs;
  PRBool mAllowPlugins;

  // Global window collection
  static nsVoidArray gBrowsers;
  static void AddBrowser(nsBrowserWindow* aBrowser);
  static void RemoveBrowser(nsBrowserWindow* aBrowser);
  static nsBrowserWindow* FindBrowserFor(nsIWidget* aWidget, PRIntn aWhich);

protected:
  nsIDOMDocument* GetDOMDocument(nsIDocShell *aDocShell);
  NS_IMETHOD EnsureWebBrowserChrome();

  nsBrowserWindow();
  virtual ~nsBrowserWindow();

  nsWebBrowserChrome*   mWebBrowserChrome;
};

// XXX This is bad; because we can't hang a closure off of the event
// callback we have no way to store our This pointer; therefore we
// have to hunt to find the browswer that events belong too!!!

// aWhich for FindBrowserFor
#define FIND_WINDOW   0
#define FIND_BACK     1
#define FIND_FORWARD  2
#define FIND_LOCATION 3

//----------------------------------------------------------------------

class nsNativeBrowserWindow : public nsBrowserWindow {
public:
  nsNativeBrowserWindow();
  ~nsNativeBrowserWindow();
  
  virtual nsresult CreateMenuBar(PRInt32 aWidth);
  virtual nsEventStatus DispatchMenuItem(PRInt32 aID);
  virtual nsresult GetMenuBarHeight(PRInt32 * aHeightOut);
  
protected:
	  NS_IMETHOD InitNativeWindow();

};

#endif /* nsBrowserWindow_h___ */
