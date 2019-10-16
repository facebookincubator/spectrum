/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

import React from 'react';
import classnames from 'classnames';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import withBaseUrl from '@docusaurus/withBaseUrl';
import styles from './styles.module.css';

const features = [
  {
    title: <>High quality output that is small</>,
    imageUrl: 'img/icon_photo_large.svg',
    description: (
      <>
      Spectrum works hard to keep your images looking great. By using <a href={'https://github.com/mozilla/mozjpeg/'}>Mozjpeg</a> it achieves a higher quality to file size ratio, common operations such as cropping and rotating can be executed lossless, and codec specifics like disabling chroma sampling increase the quality for graphical content.
      </>
    ),
  },
  {
    title: <>Simple to use and easy to learn</>,
    imageUrl: 'img/icon_code_phone.svg',
    description: (
      <>
          Because Spectrum's API is declarative, you define the output characteristics of the image and Spectrum does the complicated orchestrating for you! This helps to prevent common missteps such as incomplete handling of EXIF metadata.
      </>
    ),
  },
  {
    title: <>Cross platform and extensible</>,
    imageUrl: 'img/icon_cpu.svg',
    description: (
      <>
          Spectrum is Open Source meaning you can fully debug it and you can easily add support for new codecs using its plugin architecture. It is built with cross platform for mobile in mind and written in portable C/C++ so that you can adopt it to your environment.
      </>
    ),
  },
];

function Home() {
  const context = useDocusaurusContext();
  const {siteConfig = {}} = context;
  return (
    <Layout
      title={`${siteConfig.title}`}
      description="Description will go into a meta tag in <head />">
      <header className={classnames('hero hero--primary', styles.heroBanner)}>
        <div className="container">
          <h1 className="hero__title">{siteConfig.title}</h1>
          <p className="hero__subtitle">{siteConfig.tagline}</p>
          <div className={styles.buttons}>
            <Link
              className={classnames(
                'button button--outline button--secondary button--lg',
                styles.getStarted,
              )}

              to={withBaseUrl('docs/doc1')}>
              Get Started
            </Link>

            <Link
                className={classnames(
                    'button button--outline button--secondary button--lg',
                    styles.getStarted,
                )}
                href={'https://github.com/facebookincubator/spectrum'}>
              Github
            </Link>
          </div>
        </div>
      </header>
      <main>
        {features && features.length && (
          <section className={styles.features}>
            <div className="container">
              <div className="row">
                {features.map(({imageUrl, title, description}, idx) => (
                  <div
                    key={idx}
                    className={classnames('col col--4', styles.feature)}>
                    {imageUrl && (
                      <div className="text--center">
                        <img
                          className={styles.featureImage}
                          src={withBaseUrl(imageUrl)}
                          alt={title}
                        />
                      </div>
                    )}
                    <h3 className="text--center">{title}</h3>
                    <p className="text--center">{description}</p>
                  </div>
                ))}
              </div>
            </div>
          </section>
        )}
      </main>
    </Layout>
  );
}

export default Home;
