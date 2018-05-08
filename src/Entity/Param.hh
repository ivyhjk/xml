<?hh // strict

namespace Ivyhjk\Xml\Entity;

use DOMElement;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * <methodName> tag concrete class.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class Param extends Tag
{
    /**
     * The XML tag name
     *
     * @var string
     */
    const string TAG_NAME = 'param';

    /**
     * Generate a new <param> tag instance.
     *
     * @param Vector<Ivyhjk\Xml\Entity\Value> $values The associated values.
     * @param DOMDocument $document The root node.
     *
     * @return void
     */
    public function __construct(private Vector<Value> $values, DOMDocument $document) : void
    {
        parent::__construct($document);
    }

    /**
     * Get the tag/DOMElement name.
     *
     * @return Vector<Ivyhjk\Xml\Entity\Value>
     */
    public function getValues() : Vector<Value>
    {
        return $this->values;
    }

    /**
     * Get the <methodCall> tag as a DOMelement.
     *
     * @return DOMElement
     */
    public function getElement() : DOMElement
    {
        $element = $this
            ->getDocument()
            ->createElement(static::TAG_NAME);

        // Append the <value> tags to <param> tag.
        foreach ($this->getValues() as $value) {
            $element->appendChild($value->getElement());
        }

        return $element;
    }

    /**
     * Generate a new Param instance from a SimpleXMLElement $node.
     *
     * @param SimpleXMLElement $node The base SimpleXMLElement
     * @param DOMDocument $document The base document
     *
     * @return void
     */
    public static function fromNode(SimpleXMLElement $node, DOMDocument $document) : Param
    {
        if ($node->getName() !== self::TAG_NAME) {
            throw new InvalidNodeException(\sprintf(
                'Missing tag "name" for node "%s".',
                static::TAG_NAME
            ));
        }

        $valueEntities = Vector{};
        $valueNodes = $node->xpath(Value::TAG_NAME);

        foreach ($valueNodes as $valueNode) {
            $valueEntity = Value::fromNode($valueNode, $document);

            $valueEntities->add($valueEntity);
        }

        return  new Param($valueEntities, $document);
    }
}
