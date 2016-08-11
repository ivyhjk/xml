<?hh // strict

namespace Ivyhjk\Xml\Entity;

use DOMElement;
use DOMDocument;

/**
 * Base class to represent a "tag", ex: <param> or <struct>.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
abstract class Tag
{
    /**
     * The XML tag name
     *
     * @var string
     */
    abstract const string TAG_NAME;

    /**
     * Generate a new "tag" instance.
     *
     * @param DOMDocument $document
     *
     * @return void
     */
    public function __construct(private DOMDocument $document) : void
    {

    }

    /**
     * Get the root DOMDocument.
     *
     * @return DOMDocument
     */
    protected function getDocument() : DOMDocument
    {
        return $this->document;
    }

    /**
     * Get the tag as a DOMElement object.
     *
     * @return DOMElement
     */
    abstract public function getElement() : DOMElement;
}
