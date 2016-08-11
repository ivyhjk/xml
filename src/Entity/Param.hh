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
     * Description of what this does.
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
            ->createElement('param');

        // Append the <value> tags to <param> tag.
        foreach ($this->getValues() as $value) {
            $element->appendChild($value->getElement());
        }

        return $element;
    }

    /**
     * Description of what this does.
     *
     * @param mixed $myVar Short description
     *
     * @return void
     */
    public static function fromNode(SimpleXMLElement $node, DOMDocument $document) : Param
    {
        if ($node->getName() !== self::TAG_NAME) {
            throw new InvalidNodeException('Missing tag "name" for node "param".');
        }

        $valueEntities = Vector{};
        $valueNodes = $node->xpath('value');

        foreach ($valueNodes as $valueNode) {
            $valueEntity = Value::fromNode($valueNode, $document);

            $valueEntities->add($valueEntity);
        }

        return  new Param($valueEntities, $document);
    }
}
