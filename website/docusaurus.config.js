/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
const repoUrl = 'https://github.com/facebookincubator/spectrum'

const siteConfig = {
  title: 'Spectrum',
  tagline: 'Spectrum is a cross-platform image transcoding library that can easily be integrated into an Android or iOS project to efficiently perform common image operations.',
  url: 'https://libspectrum.io/',
  baseUrl: '/',
  favicon: 'img/favicon.png',
  // Used for publishing and more
  projectName: 'spectrum',
  organizationName: 'facebookincubator',
  themeConfig: {
    navbar: {
      title: 'Spectrum',
      logo: {
        alt: 'Spectrum Logo',
        src: 'img/spectrum_logo_square.png',
      },
      links: [
        {to: 'docs/about_spectrum', label: 'About Spectrum', position: 'right'},
        {to: 'docs/getting_started_android', label: 'Getting Started', position: 'right'},
        {
          href: 'https://github.com/facebook/docusaurus',
          label: 'GitHub',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Docs',
          items: [
            {to: 'docs/about_spectrum', label: 'About Spectrum'},
            {to: 'docs/getting_started_android', label: 'Getting Started'},
            {to: 'docs/contributing_android', label: 'Contributing'},
          ],
        },
        {
          title: 'Community',
          items: [
            {
              label: 'Facebook',
              href: 'https://www.facebook.com/libspectrum',
            },
            {
              label: 'Twitter',
              href: 'https://twitter.com/libspectrum"',
            },
          ],
        },
        {
          title: 'More',
          items: [
            {
              href: 'https://github.com/facebookincubator/spectrum',
              label: 'GitHub',
            },
          ],
        },
      ],
      logo: {
        alt: 'Facebook Open Source Logo',
        src: 'https://docusaurus.io/img/oss_logo.png',
      },
      repoUrl,
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
      copyright: `Copyright © ${new Date().getFullYear()} Facebook.`,
    },
  },
  presets: [
    [
      '@docusaurus/preset-classic',
      {
        docs: {
          sidebarPath: require.resolve('./sidebars.js'),
        },
        theme: {
          customCss: require.resolve('./src/css/custom.css'),
        },
      },
    ],
  ],
};
module.exports = siteConfig;
