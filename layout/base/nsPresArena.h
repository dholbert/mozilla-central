/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=2 sw=2 et tw=78:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef nsPresArena_h___
#define nsPresArena_h___

#include "mozilla/MemoryChecking.h"
#include "mozilla/StandardInteger.h"
#include "nscore.h"
#include "nsQueryFrame.h"
#include "nsTArray.h"
#include "nsTHashtable.h"
#include "plarena.h"

struct nsArenaMemoryStats;

class nsPresArena {
public:
  nsPresArena();
  ~nsPresArena();

  // Pool allocation with recycler lists indexed by object size, aSize.
  NS_HIDDEN_(void*) AllocateBySize(size_t aSize);
  NS_HIDDEN_(void)  FreeBySize(size_t aSize, void* aPtr);

  // Pool allocation with recycler lists indexed by frame-type ID.
  // Every aID must always be used with the same object size, aSize.
  NS_HIDDEN_(void*) AllocateByFrameID(nsQueryFrame::FrameIID aID, size_t aSize);
  NS_HIDDEN_(void)  FreeByFrameID(nsQueryFrame::FrameIID aID, void* aPtr);

  enum ObjectID {
    nsLineBox_id = nsQueryFrame::NON_FRAME_MARKER,
    nsRuleNode_id,
    nsStyleContext_id,
    nsFrameList_id,

    // The PresArena implementation uses this bit to distinguish objects
    // allocated by size from objects allocated by type ID (that is, frames
    // using AllocateByFrameID and other objects using AllocateByObjectID).
    // It should not collide with any Object ID (above) or frame ID (in
    // nsQueryFrame.h).  It is not 0x80000000 to avoid the question of
    // whether enumeration constants are signed.
    NON_OBJECT_MARKER = 0x40000000
  };

  // Pool allocation with recycler lists indexed by object-type ID (see above).
  // Every aID must always be used with the same object size, aSize.
  NS_HIDDEN_(void*) AllocateByObjectID(ObjectID aID, size_t aSize);
  NS_HIDDEN_(void)  FreeByObjectID(ObjectID aID, void* aPtr);

  /**
   * Fill aArenaStats with sizes of interesting objects allocated in
   * this arena and its mOther field with the size of everything else.
   */
  void SizeOfExcludingThis(nsMallocSizeOfFun aMallocSizeOf,
                           nsArenaMemoryStats* aArenaStats);

  /**
   * Get the poison value that can be used to fill a memory space with
   * an address that leads to a safe crash when dereferenced.
   *
   * The caller is responsible for ensuring that a pres shell has been
   * initialized before calling this.
   */
  static uintptr_t GetPoisonValue();

private:
  void* Allocate(uint32_t aCode, size_t aSize);
  void Free(uint32_t aCode, void* aPtr);

  // All keys to this hash table fit in 32 bits (see below) so we do not
  // bother actually hashing them.
  class FreeList : public PLDHashEntryHdr
  {
  public:
    typedef uint32_t KeyType;
    nsTArray<void *> mEntries;
    size_t mEntrySize;
    size_t mEntriesEverAllocated;

    typedef const void* KeyTypePointer;
    KeyTypePointer mKey;

    FreeList(KeyTypePointer aKey)
    : mEntrySize(0), mEntriesEverAllocated(0), mKey(aKey) {}
    // Default copy constructor and destructor are ok.

    bool KeyEquals(KeyTypePointer const aKey) const
    { return mKey == aKey; }

    static KeyTypePointer KeyToPointer(KeyType aKey)
    { return NS_INT32_TO_PTR(aKey); }

    static PLDHashNumber HashKey(KeyTypePointer aKey)
    { return NS_PTR_TO_INT32(aKey); }

    enum { ALLOW_MEMMOVE = false };
  };

#if defined(MOZ_HAVE_MEM_CHECKS)
  static PLDHashOperator UnpoisonFreeList(FreeList* aEntry, void*);
#endif
  static PLDHashOperator FreeListEnumerator(FreeList* aEntry, void* aData);
  static size_t SizeOfFreeListEntryExcludingThis(FreeList* aEntry,
                                                 nsMallocSizeOfFun aMallocSizeOf,
                                                 void*);
  size_t SizeOfIncludingThisFromMalloc(nsMallocSizeOfFun aMallocSizeOf) const;

  nsTHashtable<FreeList> mFreeLists;
  PLArenaPool mPool;
};

#endif
