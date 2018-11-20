/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

package com.facebook.spectrum.utils;

import javax.annotation.Nullable;

/** Light-weight implementation for Precondition checks. */
public final class Preconditions {

  private Preconditions() {}

  /**
   * Asserts that the given reference is not null.
   *
   * @param reference A reference that might be null
   * @return reference if reference is not <code>null</code>
   * @throws NullPointerException if the given reference is <code>null</code>
   */
  public static <T> T checkNotNull(@Nullable final T reference) {
    if (reference == null) {
      throw new NullPointerException();
    }
    return reference;
  }

  /**
   * Asserts that the given assertion about an argument is true.
   *
   * @param assertion The assertion to check
   * @throws IllegalArgumentException if the given assertion is <code>false</code>
   */
  public static void checkArgument(final boolean assertion) {
    if (!assertion) {
      throw new IllegalArgumentException();
    }
  }

  /**
   * Asserts that the given assertion about a state is true.
   *
   * @param assertion The assertion to check
   * @throws IllegalStateException if the given assertion is <code>false</code>
   */
  public static void checkState(final boolean assertion) {
    if (!assertion) {
      throw new IllegalStateException();
    }
  }
}
