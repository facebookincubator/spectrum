/*
 *  Copyright (c) 2018-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */
package com.facebook.jni;

import com.facebook.jni.annotations.DoNotStrip;
import java.util.Iterator;
import java.util.Map;
import javax.annotation.Nullable;

/**
 * To iterate over a Map from C++ requires four calls per entry: hasNext(), next(), getKey(),
 * getValue(). This helper reduces it to one call and two field gets per entry. It does not use a
 * generic argument, since in C++, the types will be erased, anyway. This is *not* a {@link
 * java.util.Iterator}.
 */
@DoNotStrip
public class MapIteratorHelper {
  @DoNotStrip private final Iterator<Map.Entry> mIterator;
  @DoNotStrip private @Nullable Object mKey;
  @DoNotStrip private @Nullable Object mValue;

  @DoNotStrip
  public MapIteratorHelper(Map map) {
    mIterator = map.entrySet().iterator();
  }

  /**
   * Moves the helper to the next entry in the map, if any. Returns true if there is an entry to
   * read.
   */
  @DoNotStrip
  boolean hasNext() {
    if (mIterator.hasNext()) {
      Map.Entry entry = mIterator.next();
      mKey = entry.getKey();
      mValue = entry.getValue();
      return true;
    } else {
      mKey = null;
      mValue = null;
      return false;
    }
  }
}
