<?hh // strict

namespace Ivyhjk\Xml\Entity;

use DOMElement;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * <methodCall> tag concrete class.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class Member extends Tag
{
    /**
     * The XML tag name
     *
     * @var string
     */
    const string TAG_NAME = 'member';

    /**
     * Generate a new <member> tag intance.
     *
     * @param string $name The member name.
     * @param Ivyhjk\Xml\Entity\Value $value The associated value.
     * @param DOMDocument The root dom document.
     *
     * @return void
     */
    public function __construct(private string $name, private Value $value, DOMDocument $document) : void
    {
        parent::__construct($document);
    }

    /**
     * Get the member tag name.
     *
     * @return string
     */
    public function getName() : string
    {
        return $this->name;
    }

    /**
     * Get the associated value tag class for member tag.
     *
     * @return Ivyhjk\Xml\Entity\Value
     */
    public function getValue() : Value
    {
        return $this->value;
    }

    /**
     * Get the <methodCall> tag as a DOMelement.
     *
     * @return DOMElement
     */
    public function getElement() : DOMElement
    {
        $element = $this->getDocument()->createElement('member');

        $name = $this->getDocument()->createElement('name', $this->getName());

        $element->appendChild($name);
        $element->appendChild($this->getValue()->getElement());

        return $element;
    }

    /**
     * Get the entity from a given node.
     *
     * @param SimpleXMLElement $node
     * @param DOMDocument $document
     *
     * @return Ivyhjk\Xml\Entity\Member
     */
    public static function fromNode(SimpleXMLElement $node, DOMDocument $document) : Member
    {
        if ($node->getName() !== self::TAG_NAME) {
            throw new InvalidNodeException('Invalid tag "name" for "member" node.');
        }

        $nameNode = (new Vector($node->xpath('name')))->firstValue();

        if ($nameNode === null) {
            throw new InvalidNodeException('Tag "name" not found into "member" node.');
        }

        $valueNode = (new Vector($node->xpath('value')))->firstValue();

        if ($valueNode === null) {
            throw new InvalidNodeException('Tag "value" not found into "member" node.');
        }

        $memberName = (string) $nameNode;
        $valueEntity = Value::fromNode($valueNode, $document);

        return new Member($memberName, $valueEntity, $document);
    }
}
