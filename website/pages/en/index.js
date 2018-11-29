/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

const React = require('react');

const CompLibrary = require('../../core/CompLibrary.js');

const MarkdownBlock = CompLibrary.MarkdownBlock; /* Used to read markdown */
const Container = CompLibrary.Container;
const GridBlock = CompLibrary.GridBlock;

const siteConfig = require(`${process.cwd()}/siteConfig.js`);

function imgUrl(img) {
  return `${siteConfig.baseUrl}img/${img}`;
}

function docUrl(doc, language) {
  return `${siteConfig.baseUrl}docs/${language ? `${language}/` : ''}${doc}`;
}

function pageUrl(page, language) {
  return siteConfig.baseUrl + (language ? `${language}/` : '') + page;
}

const SplashContainer = props => (
  <div className="homeContainer shadow">
    <div className="homeSplashFade">
      <div className="wrapper homeWrapper">{props.children}</div>
    </div>
  </div>
);

const Logo = props => (
  <div className="projectLogo">
    <img src={props.img_src} alt="Project Logo" />
  </div>
);

const ProjectTitle = () => (
  <h2 className="projectTitle inverse">
    {siteConfig.title}
  </h2>
);

class HomeSplash extends React.Component {
  render() {
    const language = this.props.language || '';
    return (
      <SplashContainer>
        <div className="content">
        <div className="inner">
          <ProjectTitle />
        </div>
        <h2 className="inverseLight">Spectrum is a cross-platform image transcoding library that can easily be integrated into an Android or iOS project to efficiently perform common image operations.</h2>
      </div>
      <div className="buttons">
        <a className="button" href="docs/getting_started_android">GET STARTED</a>
        <a className="button" href="https://github.com/facebookincubator/spectrum">GITHUB</a>
      </div>

      </SplashContainer>
    );
  }
}

const Block = props => (
  <Container
    padding={['bottom', 'top']}
    id={props.id}
    background={props.background}>
    <GridBlock align="center" contents={props.children} layout={props.layout} />
  </Container>
);

const Features = () => (
  <Block layout="fourColumn">
    {[
      {
        content: 'Spectrum works hard to keep your images looking great. By using [Mozjpeg](https://github.com/mozilla/mozjpeg/) it achieves a higher quality to file size ratio, common operations such as cropping and rotating can be executed lossless, and codec specifics like disabling chroma sampling increase the quality for graphical content.',
        image: imgUrl('icon_photo_large.svg'),
        imageAlign: 'top',
        title: 'High quality output that is small',
      },
      {
        content: 'Because Spectrum\'s API is declarative, you define the output characteristics of the image and Spectrum does the complicated orchestrating for you! This helps to prevent common missteps such as incomplete handling of EXIF metadata.',
        image: imgUrl('icon_code_phone.svg'),
        imageAlign: 'top',
        title: 'Simple to use and easy to learn',
      },
      {
        content: 'Spectrum is Open Source meaning you can fully debug it and you can easily add support for new codecs using its plugin architecture. It is built with cross platform for mobile in mind  and written in portable C/C++ so that you can adopt it to your environment.',
        image: imgUrl('icon_cpu.svg'),
        imageAlign: 'top',
        title: 'Cross platform and extensible',
      },
    ]}
  </Block>
);

class Index extends React.Component {
  render() {
    const language = this.props.language || '';
    return (
      <div>
        <HomeSplash language={language} />
        <div className="mainContainer">
          <Features />
        </div>
      </div>
    );
  }
}

module.exports = Index;
