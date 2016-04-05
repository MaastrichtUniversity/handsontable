# Handsontable element for Islandora XML Forms (Drupal)

## Introduction

This is an extenstion to [Islandora XML forms](https://github.com/Islandora/islandora_xml_forms)
(part of [Islandora](http://islandora.ca/)) which allows
to run [Handsontable](https://handsontable.com/) as a Drupal form element.

Handsontable is a Javascript extension that provides an Excel-like interface (grid) within the web browser.
This extension integrates handsontable with Islandora XML forms. You can define columns which will
end up as XML element in the resulting document.

![Example](https://cloud.githubusercontent.com/assets/6281107/14286982/67336346-fb51-11e5-88cc-44a769d178ae.png)
![Example](https://cloud.githubusercontent.com/assets/6281107/14286983/6734385c-fb51-11e5-8628-37f510ff98b3.png)

## Requirements

This Drupal module requires the following Drupal modules/libraries:

* [Islandora XML forms](https://github.com/Islandora/islandora_xml_forms)
* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [phplib](https://github.com/islandora/php_lib)
* [Objective forms](https://github.com/islandora/objective_forms)

Additionally, this PHP extension needs to be enabled
* [libxml2](http://xmlsoft.org/) version 2.7+

## Installation

* Download the zip file from the [release page](https://github.com/MaastrichtUniversity/handsontable/releases).
* Unpack it in the `sites/all/modules/contrib` directory.
* Enable the module on the Administration -> Module page.

Alternatively, you can download directly from the source repository. You need to use
[bower](http://bower.io/) to get all Javascript dependencies. Like this:

 ```
 cd sites/all/modules/contrib
 git clone https://github.com/MaastrichtUniversity/handsontable.git
 cd handsontable
 bower install
 ```

## Configuration

The basic configuration is shown in this screenshot.

![Configuration](https://cloud.githubusercontent.com/assets/6281107/14286516/9173b720-fb4f-11e5-9429-125b646f7e5d.png)

In short, create a handsontable element and as a child a handsontable_row element. The children of the row element
will be columns of the table. These can be text fields.
