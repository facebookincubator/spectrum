/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

const siteConfig = {
  title: 'Spectrum',
  tagline: 'Making mobile image processing simple',
  url: 'https://libspectrum.io/',
  baseUrl: '/',
  repoUrl: 'https://github.com/facebookincubator/spectrum',

  // Used for publishing and more
  projectName: 'spectrum',
  organizationName: 'facebookincubator',

  // For no header links in the top nav bar -> headerLinks: [],
  headerLinks: [
    {doc: 'about_spectrum', label: 'About Spectrum'},
    {doc: 'getting_started_android', label: 'Getting Started'},
    {href: 'https://github.com/facebookincubator/spectrum', label: 'GitHub'},
  ],

  /* path to images for header/footer */
  headerIcon: 'img/spectrum_logo_square.png',
  footerIcon: 'img/spectrum_logo_white.svg',
  favicon: 'img/favicon.png',

  /* Colors for website */
  colors: {
    primaryColor: '#B9459A',
    secondaryColor: '#8A3373',
  },

  // This copyright info is used in /core/Footer.js and blog RSS/Atom feeds.
  copyright: `Copyright © ${new Date().getFullYear()} Facebook`,

  usePrism: ['java', 'objective-c', 'groovy', 'bash'],
  highlight: {
    theme: 'atom-one-dark',
  },

  // Add custom scripts here that would be placed in <script> tags.
  scripts: ['https://buttons.github.io/buttons.js'],

  // On page navigation for the current documentation page.
  onPageNav: 'separate',
  // No .html extensions for paths.
  cleanUrl: true,
};

module.exports = siteConfig;
